/*
 * raw.c - Raw sockets for protocol family CAN
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, the following disclaimer and
 *    the referenced file 'COPYING'.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2 as distributed in the 'COPYING'
 * file from the main directory of the linux kernel source.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <socketcan-users@lists.berlios.de>
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/uio.h>
#include <linux/poll.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/if_arp.h>
#include <linux/skbuff.h>
#include <linux/can.h>
#include <linux/can/core.h>
#include <linux/can/raw.h>
#include <net/sock.h>

#define IDENT "raw"
#define CAN_RAW_VERSION CAN_VERSION
static __initdata const char banner[] =
	KERN_INFO "can: raw protocol (rev " CAN_RAW_VERSION ")\n";

MODULE_DESCRIPTION("PF_CAN raw protocol");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Urs Thuermann <urs.thuermann@volkswagen.de>");

#ifdef CONFIG_CAN_DEBUG_CORE
static int debug;
module_param(debug, int, S_IRUGO);
MODULE_PARM_DESC(debug, "debug print mask: 1:debug, 2:frames, 4:skbs");
#endif

#ifdef CONFIG_CAN_RAW_USER
#define RAW_CAP (-1)
#else
#define RAW_CAP CAP_NET_RAW
#endif

#define MASK_ALL 0

/*
 * A raw socket has a list of can_filters attached to it, each receiving
 * the CAN frames matching that filter.  If the filter list is empty,
 * no CAN frames will be received by the socket.  The default after
 * opening the socket, is to have one filter which receives all frames.
 * The filter list is allocated dynamically with the exception of the
 * list containing only one item.  This common case is optimized by
 * storing the single filter in dfilter, to avoid using dynamic memory.
 */

struct raw_sock {
	struct sock sk;
	int bound;
	int ifindex;
	struct notifier_block notifier;
	int loopback;
	int recv_own_msgs;
	int count;                 /* number of active filters */
	struct can_filter dfilter; /* default/single filter */
	struct can_filter *filter; /* pointer to filter(s) */
	can_err_mask_t err_mask;
};

static inline struct raw_sock *raw_sk(const struct sock *sk)
{
	return (struct raw_sock *)sk;
}

static void raw_rcv(struct sk_buff *skb, void *data)
{
	struct sock *sk = (struct sock *)data;
	struct raw_sock *ro = raw_sk(sk);
	struct sockaddr_can *addr;
	int error;

	DBG("received skbuff %p, sk %p\n", skb, sk);
	DBG_SKB(skb);

	if (!ro->recv_own_msgs) {
		/* check the received tx sock reference */
		if (skb->sk == sk) {
			DBG("trashed own tx msg\n");
			kfree_skb(skb);
			return;
		}
	}

	addr = (struct sockaddr_can *)skb->cb;
	memset(addr, 0, sizeof(*addr));
	addr->can_family  = AF_CAN;
	addr->can_ifindex = skb->dev->ifindex;

	error = sock_queue_rcv_skb(sk, skb);
	if (error < 0) {
		DBG("sock_queue_rcv_skb failed: %d\n", error);
		DBG("freeing skbuff %p\n", skb);
		kfree_skb(skb);
	}
}

static void raw_enable_filters(struct net_device *dev, struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);
	struct can_filter *filter = ro->filter;
	int i;

	for (i = 0; i < ro->count; i++) {
		DBG("filter can_id %08X, can_mask %08X%s, sk %p\n",
		    filter[i].can_id, filter[i].can_mask,
		    filter[i].can_id & CAN_INV_FILTER ? " (inv)" : "", sk);

		can_rx_register(dev, filter[i].can_id, filter[i].can_mask,
				raw_rcv, sk, IDENT);
	}
}

static void raw_enable_errfilter(struct net_device *dev, struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);

	if (ro->err_mask)
		can_rx_register(dev, 0, ro->err_mask | CAN_ERR_FLAG,
				raw_rcv, sk, IDENT);
}

static void raw_disable_filters(struct net_device *dev, struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);
	struct can_filter *filter = ro->filter;
	int i;

	for (i = 0; i < ro->count; i++) {
		DBG("filter can_id %08X, can_mask %08X%s, sk %p\n",
		    filter[i].can_id, filter[i].can_mask,
		    filter[i].can_id & CAN_INV_FILTER ? " (inv)" : "", sk);

		can_rx_unregister(dev, filter[i].can_id, filter[i].can_mask,
				  raw_rcv, sk);
	}
}

static void raw_disable_errfilter(struct net_device *dev, struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);

	if (ro->err_mask)
		can_rx_unregister(dev, 0, ro->err_mask | CAN_ERR_FLAG,
				  raw_rcv, sk);
}

static int raw_notifier(struct notifier_block *nb,
			unsigned long msg, void *data)
{
	struct net_device *dev = (struct net_device *)data;
	struct raw_sock *ro = container_of(nb, struct raw_sock, notifier);
	struct sock *sk = &ro->sk;

	DBG("msg %ld for dev %p (%s idx %d) sk %p ro->ifindex %d\n",
	    msg, dev, dev->name, dev->ifindex, sk, ro->ifindex);

	if (dev->type != ARPHRD_CAN)
		return NOTIFY_DONE;

	if (ro->ifindex != dev->ifindex)
		return NOTIFY_DONE;

	switch (msg) {

	case NETDEV_UNREGISTER:
		lock_sock(sk);
		/* remove current filters & unregister */
		if (ro->bound) {
			raw_disable_filters(dev, sk);
			raw_disable_errfilter(dev, sk);
		}

		if (ro->count > 1)
			kfree(ro->filter);

		ro->ifindex = 0;
		ro->bound   = 0;
		ro->count   = 0;
		release_sock(sk);

		sk->sk_err = ENODEV;
		if (!sock_flag(sk, SOCK_DEAD))
			sk->sk_error_report(sk);
		break;

	case NETDEV_DOWN:
		sk->sk_err = ENETDOWN;
		if (!sock_flag(sk, SOCK_DEAD))
			sk->sk_error_report(sk);
		break;
	}

	return NOTIFY_DONE;
}

static int raw_init(struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);

	ro->bound            = 0;
	ro->ifindex          = 0;

	/* set default filter to single entry dfilter */
	ro->dfilter.can_id   = 0;
	ro->dfilter.can_mask = MASK_ALL;
	ro->filter           = &ro->dfilter;
	ro->count            = 1;

	/* set default loopback behaviour */
	ro->loopback         = 1;
	ro->recv_own_msgs    = 0;

	/* set notifier */
	ro->notifier.notifier_call = raw_notifier;

	register_netdevice_notifier(&ro->notifier);

	return 0;
}

static int raw_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);

	DBG("socket %p, sk %p, refcnt %d\n", sock, sk,
	    atomic_read(&sk->sk_refcnt));

	unregister_netdevice_notifier(&ro->notifier);

	lock_sock(sk);

	/* remove current filters & unregister */
	if (ro->bound) {
		if (ro->ifindex) {
			struct net_device *dev = dev_get_by_index(ro->ifindex);
			if (dev) {
				raw_disable_filters(dev, sk);
				raw_disable_errfilter(dev, sk);
				dev_put(dev);
			}
		} else {
			raw_disable_filters(NULL, sk);
			raw_disable_errfilter(NULL, sk);
		}
	}

	if (ro->count > 1)
		kfree(ro->filter);

	ro->ifindex = 0;
	ro->bound   = 0;
	ro->count   = 0;

	release_sock(sk);
	sock_put(sk);

	return 0;
}

static int raw_bind(struct socket *sock, struct sockaddr *uaddr, int len)
{
	struct sockaddr_can *addr = (struct sockaddr_can *)uaddr;
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	int err = 0;
	int notify_enetdown = 0;

	DBG("socket %p to device %d\n", sock, addr->can_ifindex);

	if (len < sizeof(*addr))
		return -EINVAL;

	lock_sock(sk);

	if (ro->bound) {
		/* unregister current filters for this device */
		if (ro->ifindex) {
			struct net_device *dev = dev_get_by_index(ro->ifindex);
			if (dev) {
				raw_disable_filters(dev, sk);
				raw_disable_errfilter(dev, sk);
				dev_put(dev);
			}
			ro->ifindex = 0;

		} else {
			raw_disable_filters(NULL, sk);
			raw_disable_errfilter(NULL, sk);
		}

		ro->bound = 0;
	}

	if (addr->can_ifindex) {
		struct net_device *dev = dev_get_by_index(addr->can_ifindex);
		if (!dev) {
			DBG("could not find device %d\n", addr->can_ifindex);
			err = -ENODEV;
			goto out;
		}
		if (dev->type != ARPHRD_CAN) {
			DBG("device %d no CAN device\n", addr->can_ifindex);
			dev_put(dev);
			err = -ENODEV;
			goto out;
		}
		if (!(dev->flags & IFF_UP))
			notify_enetdown = 1;

		ro->ifindex = dev->ifindex;

		/* filters set by default/setsockopt */
		raw_enable_filters(dev, sk);
		raw_enable_errfilter(dev, sk);
		dev_put(dev);

	} else {
		ro->ifindex = 0;

		/* filters set by default/setsockopt */
		raw_enable_filters(NULL, sk);
		raw_enable_errfilter(NULL, sk);
	}

	ro->bound = 1;

 out:
	release_sock(sk);

	if (notify_enetdown) {
		sk->sk_err = ENETDOWN;
		if (!sock_flag(sk, SOCK_DEAD))
			sk->sk_error_report(sk);
	}

	return err;
}

static int raw_getname(struct socket *sock, struct sockaddr *uaddr,
		       int *len, int peer)
{
	struct sockaddr_can *addr = (struct sockaddr_can *)uaddr;
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);

	if (peer)
		return -EOPNOTSUPP;

	addr->can_family  = AF_CAN;
	addr->can_ifindex = ro->ifindex;

	*len = sizeof(*addr);

	return 0;
}

static unsigned int raw_poll(struct file *file, struct socket *sock,
			     poll_table *wait)
{
	unsigned int mask = 0;

	DBG("socket %p\n", sock);

	mask = datagram_poll(file, sock, wait);
	return mask;
}

static int raw_setsockopt(struct socket *sock, int level, int optname,
			  char __user *optval, int optlen)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	struct can_filter *filter = NULL;  /* dyn. alloc'ed filters */
	struct can_filter sfilter;         /* single filter */
	struct net_device *dev = NULL;
	can_err_mask_t err_mask = 0;
	int count = 0;
	int err;

	if (level != SOL_CAN_RAW)
		return -EINVAL;
	if (optlen < 0)
		return -EINVAL;

	switch (optname) {

	case CAN_RAW_FILTER:
		if (optlen % sizeof(struct can_filter) != 0)
			return -EINVAL;

		count = optlen / sizeof(struct can_filter);

		if (count > 1) {
			/* filter does not fit into dfilter => alloc space */
			filter = kmalloc(optlen, GFP_KERNEL);
			if (!filter)
				return -ENOMEM;

			err = copy_from_user(filter, optval, optlen);
			if (err) {
				kfree(filter);
				return err;
			}
		} else if (count == 1) {
			err = copy_from_user(&sfilter, optval, optlen);
			if (err)
				return err;
		}

		lock_sock(sk);

		if (ro->bound && ro->ifindex)
			dev = dev_get_by_index(ro->ifindex);

		/* remove current filters & unregister */
		if (ro->bound)
			raw_disable_filters(dev, sk);

		if (ro->count > 1)
			kfree(ro->filter);

		if (count == 1) {
			/* copy filter data for single filter */
			ro->dfilter = sfilter;
			filter = &ro->dfilter;
		}

		/* add new filters & register */
		ro->filter = filter;
		ro->count  = count;
		if (ro->bound)
			raw_enable_filters(dev, sk);

		if (dev)
			dev_put(dev);

		release_sock(sk);

		break;

	case CAN_RAW_ERR_FILTER:
		if (optlen != sizeof(err_mask))
			return -EINVAL;

		err = copy_from_user(&err_mask, optval, optlen);
		if (err)
			return err;

		err_mask &= CAN_ERR_MASK;

		lock_sock(sk);

		if (ro->bound && ro->ifindex)
			dev = dev_get_by_index(ro->ifindex);

		/* remove current error mask */
		if (ro->bound)
			raw_disable_errfilter(dev, sk);

		ro->err_mask = err_mask;

		/* add new error mask */
		if (ro->bound)
			raw_enable_errfilter(dev, sk);

		if (dev)
			dev_put(dev);

		release_sock(sk);

		break;

	case CAN_RAW_LOOPBACK:
		if (optlen != sizeof(ro->loopback))
			return -EINVAL;

		err = copy_from_user(&ro->loopback, optval, optlen);
		if (err)
			return err;

		break;

	case CAN_RAW_RECV_OWN_MSGS:
		if (optlen != sizeof(ro->recv_own_msgs))
			return -EINVAL;

		err = copy_from_user(&ro->recv_own_msgs, optval, optlen);
		if (err)
			return err;

		break;

	default:
		return -ENOPROTOOPT;
	}
	return 0;
}

static int raw_getsockopt(struct socket *sock, int level, int optname,
			  char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	int len;
	void *val;
	int err = 0;

	if (level != SOL_CAN_RAW)
		return -EINVAL;
	if (get_user(len, optlen))
		return -EFAULT;
	if (len < 0)
		return -EINVAL;

	switch (optname) {

	case CAN_RAW_FILTER:
		lock_sock(sk);
		if (ro->count > 0) {
			int fsize = ro->count * sizeof(struct can_filter);
			if (len > fsize)
				len = fsize;
			err = copy_to_user(optval, ro->filter, len);
		} else
			len = 0;
		release_sock(sk);

		if (!err)
			err = put_user(len, optlen);
		return err;

	case CAN_RAW_ERR_FILTER:
		if (len > sizeof(can_err_mask_t))
			len = sizeof(can_err_mask_t);
		val = &ro->err_mask;
		break;

	case CAN_RAW_LOOPBACK:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->loopback;
		break;

	case CAN_RAW_RECV_OWN_MSGS:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->recv_own_msgs;
		break;

	default:
		return -ENOPROTOOPT;
	}

	if (put_user(len, optlen))
		return -EFAULT;
	if (copy_to_user(optval, val, len))
		return -EFAULT;
	return 0;
}

static int raw_sendmsg(struct kiocb *iocb, struct socket *sock,
		       struct msghdr *msg, size_t size)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	struct sk_buff *skb;
	struct net_device *dev;
	int ifindex;
	int err;

	DBG("socket %p, sk %p\n", sock, sk);

	if (msg->msg_name) {
		struct sockaddr_can *addr =
			(struct sockaddr_can *)msg->msg_name;

		if (addr->can_family != AF_CAN)
			return -EINVAL;

		ifindex = addr->can_ifindex;
	} else
		ifindex = ro->ifindex;

	dev = dev_get_by_index(ifindex);
	if (!dev) {
		DBG("device %d not found\n", ifindex);
		return -ENXIO;
	}

	skb = alloc_skb(size, GFP_KERNEL);
	if (!skb) {
		dev_put(dev);
		return -ENOMEM;
	}

	err = memcpy_fromiovec(skb_put(skb, size), msg->msg_iov, size);
	if (err < 0) {
		kfree_skb(skb);
		dev_put(dev);
		return err;
	}
	skb->dev = dev;
	skb->sk  = sk;

	DBG("sending skbuff to interface %d\n", ifindex);
	DBG_SKB(skb);

	err = can_send(skb, ro->loopback);

	dev_put(dev);

	if (err)
		return err;

	return size;
}

static int raw_recvmsg(struct kiocb *iocb, struct socket *sock,
		       struct msghdr *msg, size_t size, int flags)
{
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int error = 0;
	int noblock;

	DBG("socket %p, sk %p\n", sock, sk);

	noblock =  flags & MSG_DONTWAIT;
	flags   &= ~MSG_DONTWAIT;

	skb = skb_recv_datagram(sk, flags, noblock, &error);
	if (!skb)
		return error;

	DBG("delivering skbuff %p\n", skb);
	DBG_SKB(skb);

	if (size < skb->len)
		msg->msg_flags |= MSG_TRUNC;
	else
		size = skb->len;

	error = memcpy_toiovec(msg->msg_iov, skb->data, size);
	if (error < 0) {
		skb_free_datagram(sk, skb);
		return error;
	}

	sock_recv_timestamp(msg, sk, skb);

	if (msg->msg_name) {
		msg->msg_namelen = sizeof(struct sockaddr_can);
		memcpy(msg->msg_name, skb->cb, msg->msg_namelen);
	}

	DBG("freeing sock %p, skbuff %p\n", sk, skb);
	skb_free_datagram(sk, skb);

	return size;
}

static struct proto_ops raw_ops = {
	.family        = PF_CAN,
	.release       = raw_release,
	.bind          = raw_bind,
	.connect       = sock_no_connect,
	.socketpair    = sock_no_socketpair,
	.accept        = sock_no_accept,
	.getname       = raw_getname,
	.poll          = raw_poll,
	.ioctl         = NULL,		/* use can_ioctl() from af_can.c */
	.listen        = sock_no_listen,
	.shutdown      = sock_no_shutdown,
	.setsockopt    = raw_setsockopt,
	.getsockopt    = raw_getsockopt,
	.sendmsg       = raw_sendmsg,
	.recvmsg       = raw_recvmsg,
	.mmap          = sock_no_mmap,
	.sendpage      = sock_no_sendpage,
};

static struct proto raw_proto = {
	.name       = "CAN_RAW",
	.owner      = THIS_MODULE,
	.obj_size   = sizeof(struct raw_sock),
	.init       = raw_init,
};

static struct can_proto raw_can_proto = {
	.type       = SOCK_RAW,
	.protocol   = CAN_RAW,
	.capability = RAW_CAP,
	.ops        = &raw_ops,
	.prot       = &raw_proto,
};

static __init int raw_module_init(void)
{
	printk(banner);

	can_proto_register(&raw_can_proto);
	return 0;
}

static __exit void raw_module_exit(void)
{
	can_proto_unregister(&raw_can_proto);
}

module_init(raw_module_init);
module_exit(raw_module_exit);
