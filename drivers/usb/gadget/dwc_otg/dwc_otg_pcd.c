/* ==========================================================================
 * $File: //dwh/usb_iip/dev/software/otg_ipmate/linux/drivers/dwc_otg_pcd.c $
 * $Revision: #6 $
 * $Date: 2005/09/27 $
 * $Change: 541783 $
 *
 * Synopsys HS OTG Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */
#ifndef DWC_HOST_ONLY

/** @file
 * This file implements the Peripheral Controller Driver.
 *
 * The Peripheral Controller Driver (PCD) is responsible for
 * translating requests from the Function Driver into the appropriate
 * actions on the DWC_otg controller. It isolates the Function Driver
 * from the specifics of the controller by providing an API to the
 * Function Driver.
 *
 * The Peripheral Controller Driver for Linux will implement the
 * Gadget API, so that the existing Gadget drivers can be used.
 * (Gadget Driver is the Linux terminology for a Function Driver.)
 *
 * The Linux Gadget API is defined in the header file
 * <code><linux/usb_gadget.h></code>.  The USB EP operations API is
 * defined in the structure <code>usb_ep_ops</code> and the USB
 * Controller API is defined in the structure
 * <code>usb_gadget_ops</code>.
 *
 * An important function of the PCD is managing interrupts generated
 * by the DWC_otg controller. The implementation of the DWC_otg device
 * mode interrupt service routines is in dwc_otg_pcd_intr.c.
 *
 * @todo Add Device Mode test modes (Test J mode, Test K mode, etc).
 * @todo Does it work when the request size is greater than DEPTSIZ
 * transfer size
 *
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>

#include <linux/usb/ch9.h>
#include <linux/usb_gadget.h>

#include "dwc_otg_driver.h"
#include "dwc_otg_pcd.h"



/**
 * Static PCD pointer for use in usb_gadget_register_driver and
 * usb_gadget_unregister_driver.  Initialized in dwc_otg_pcd_init.
 */
static   dwc_otg_pcd_t *s_pcd = 0;


/* Display the contents of the buffer */
extern void dump_msg(const u8 *buf, unsigned int length);


/**
 * This function completes a request.  It call's the request call back.
 */
void request_done(dwc_otg_pcd_ep_t *_ep, dwc_otg_pcd_request_t *_req,
                  int _status)
{
	unsigned stopped = _ep->stopped;

        DWC_DEBUGPL(DBG_PCDV, "%s(%p)\n", __func__, _ep);
	list_del_init(&_req->queue);

	if (_req->req.status == -EINPROGRESS) {
		_req->req.status = _status;
        } else {
                _status = _req->req.status;
        }

	/* don't modify queue heads during completion callback */
	_ep->stopped = 1;
        SPIN_UNLOCK(&_ep->pcd->lock);
	_req->req.complete(&_ep->ep, &_req->req);
        SPIN_LOCK(&_ep->pcd->lock);

        if (_ep->pcd->request_pending > 0){
                --_ep->pcd->request_pending;
        }

	_ep->stopped = stopped;

	/*
	 * Added-sr: 2007-07-26
	 *
	 * Finally, when the current request is done, mark this endpoint
	 * as not active, so that new requests can be processed.
	 */
	_ep->dwc_ep.active = 0;
}

/**
 * This function terminates all the requsts in the EP request queue.
 */
void request_nuke( dwc_otg_pcd_ep_t *_ep )
{
	dwc_otg_pcd_request_t *req;

        _ep->stopped = 1;

	/* called with irqs blocked?? */
	while (!list_empty(&_ep->queue)) {
		req = list_entry(_ep->queue.next, dwc_otg_pcd_request_t,
                                 queue);
		request_done(_ep, req, -ESHUTDOWN );
	}
}

/* USB Endpoint Operations */
/*
 * The following sections briefly describe the behavior of the Gadget
 * API endpoint operations implemented in the DWC_otg driver
 * software. Detailed descriptions of the generic behavior of each of
 * these functions can be found in the Linux header file
 * include/linux/usb_gadget.h.
 *
 * The Gadget API provides wrapper functions for each of the function
 * pointers defined in usb_ep_ops. The Gadget Driver calls the wrapper
 * function, which then calls the underlying PCD function. The
 * following sections are named according to the wrapper
 * functions. Within each section, the corresponding DWC_otg PCD
 * function name is specified.
 *
 */

/**
 * This function is called by the Gadget Driver for each EP to be
 * configured for the current configuration (SET_CONFIGURATION).
 *
 * This function initializes the dwc_otg_ep_t data structure, and then
 * calls dwc_otg_ep_activate.
 */
static int dwc_otg_pcd_ep_enable(struct usb_ep *_ep,
                                 const struct usb_endpoint_descriptor *_desc)
{
        dwc_otg_pcd_ep_t *ep = 0;
        dwc_otg_pcd_t *pcd = 0;
        unsigned long flags;
        DWC_DEBUGPL(DBG_PCDV,"%s(%p,%p)\n", __func__, _ep, _desc );

        ep = container_of(_ep, dwc_otg_pcd_ep_t, ep);
	if (!_ep || !_desc || ep->desc ||
            _desc->bDescriptorType != USB_DT_ENDPOINT) {
		DWC_WARN( "%s, bad ep or descriptor\n", __func__);
		return -EINVAL;
        }
	if (ep == &ep->pcd->ep[0]){
		DWC_WARN("%s, bad ep(0)\n", __func__);
		return -EINVAL;
        }

        /* Check FIFO size? */
	if (!_desc->wMaxPacketSize) {
		DWC_WARN("%s, bad %s maxpacket\n", __func__, _ep->name);
		return -ERANGE;
	}

        pcd = ep->pcd;
	if (!pcd->driver || pcd->gadget.speed == USB_SPEED_UNKNOWN) {
		DWC_WARN("%s, bogus device state\n", __func__);
		return -ESHUTDOWN;
	}

        SPIN_LOCK_IRQSAVE(&pcd->lock, flags);

	ep->desc = _desc;
	ep->ep.maxpacket = le16_to_cpu(_desc->wMaxPacketSize);

        /*
         * Activate the EP
         */
	/* DFX TODO Why isn't the ep->dwc_ep.active flag set to true here? */
        ep->stopped = 0;
        ep->dwc_ep.is_in = (USB_DIR_IN & _desc->bEndpointAddress) != 0;
        ep->dwc_ep.maxpacket = ep->ep.maxpacket;
        ep->dwc_ep.tx_fifo_num = 0;
        ep->dwc_ep.type = _desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
        if ((_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
            USB_ENDPOINT_XFER_ISOC ) {
                /*
                 * if ISOC EP then assign a Periodic Tx FIFO.
                 */
                /** @todo NGS Determine Tx FIFO for periodic EP.
                 *  Currently using 1.
                 */
                ep->dwc_ep.tx_fifo_num = 1;
        }
        /* Set initial data PID. */
        if ((_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
            USB_ENDPOINT_XFER_BULK ) {
                ep->dwc_ep.data_pid_start = 0;
        }

        DWC_DEBUGPL(DBG_PCD, "Activate %s-%s: type=%d, mps=%d desc=%p\n",
                    ep->ep.name, (ep->dwc_ep.is_in ?"IN":"OUT"),
                    ep->dwc_ep.type, ep->dwc_ep.maxpacket, ep->desc );

        dwc_otg_ep_activate( GET_CORE_IF(pcd), &ep->dwc_ep );
        SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);
        return 0;
}

/**
 * This function is called when an EP is disabled due to disconnect or
 * change in configuration. Any pending requests will terminate with a
 * status of -ESHUTDOWN.
 *
 * This function modifies the dwc_otg_ep_t data structure for this EP,
 * and then calls dwc_otg_ep_deactivate.
 */
static int dwc_otg_pcd_ep_disable(struct usb_ep *_ep)
{
        dwc_otg_pcd_ep_t *ep;
        unsigned long flags;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n", __func__, _ep);
        ep = container_of(_ep, dwc_otg_pcd_ep_t, ep);
	if (!_ep || !ep->desc) {
		DWC_DEBUGPL(DBG_PCD, "%s, %s not enabled\n", __func__,
                            _ep ? ep->ep.name : NULL);
		return -EINVAL;
	}

        SPIN_LOCK_IRQSAVE(&ep->pcd->lock, flags);

        request_nuke( ep );

        dwc_otg_ep_deactivate( GET_CORE_IF(ep->pcd), &ep->dwc_ep );
	ep->desc = 0;
	ep->stopped = 1;
        SPIN_UNLOCK_IRQRESTORE(&ep->pcd->lock, flags);

	DWC_DEBUGPL(DBG_PCD, "%s disabled\n", _ep->name);
        return 0;
}


/**
 * This function allocates a request object to use with the specified
 * endpoint.
 *
 * @param _ep The endpoint to be used with with the request
 * @param _gfp_flags the GFP_* flags to use.
 */
static struct usb_request *dwc_otg_pcd_alloc_request(struct usb_ep *_ep,
                                                     gfp_t _gfp_flags)
{
        dwc_otg_pcd_request_t *req;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p,%d)\n", __func__, _ep, _gfp_flags);
        if (0 == _ep ) {
                DWC_WARN("%s() %s\n", __func__, "Invalid EP!\n");
                return 0;
        }
        req = kmalloc( sizeof(dwc_otg_pcd_request_t), _gfp_flags);
        if (0 == req){
                DWC_WARN("%s() %s\n", __func__,
                         "request allocation failed!\n");
                return 0;
        }
        memset(req, 0, sizeof(dwc_otg_pcd_request_t));
	req->req.dma = DMA_ADDR_INVALID;
	INIT_LIST_HEAD(&req->queue);
	return &req->req;
}

/**
 * This function frees a request object.
 *
 * @param _ep The endpoint associated with the request
 * @param _req The request being freed
 */
static void dwc_otg_pcd_free_request(struct usb_ep *_ep,
                                     struct usb_request *_req)
{
        dwc_otg_pcd_request_t *req;
        DWC_DEBUGPL(DBG_PCDV,"%s(%p,%p)\n", __func__, _ep, _req);

	if (0 == _ep || 0 == _req) {
                DWC_WARN("%s() %s\n", __func__,
                         "Invalid ep or req argument!\n");
		return;
        }

	req = container_of(_req, dwc_otg_pcd_request_t, req);
	kfree(req);
}

/**
 * This function is used to submit an I/O Request to an EP.
 *
 * 	- When the request completes the request's completion callback
 * 	  is called to return the request to the driver.
 *	- An EP, except control EPs, may have multiple requests
 *	  pending.
 *	- Once submitted the request cannot be examined or modified.
 *	- Each request is turned into one or more packets.
 *	- A BULK EP can queue any amount of data; the transfer is
 *	  packetized.
 *	- Zero length Packets are specified with the request 'zero'
 *	  flag.
 */
static int dwc_otg_pcd_ep_queue(struct usb_ep *_ep,
                                struct usb_request *_req, gfp_t _gfp_flags)
{
	dwc_otg_pcd_request_t *req;
	dwc_otg_pcd_ep_t *ep;
	dwc_otg_pcd_t	*pcd;
	unsigned long flags = 0;
	//static int in_count = 1;  /* DFX TODO remove these debug aids. */
	//static int out_count = 1;

        // DFX DWC_DEBUGPL(DBG_PCDV,"%s(%p,%p,%d)\n",
        DWC_DEBUGPL(DBG_PCD,"%s(%p,%p,%d)\n",
                    __func__, _ep, _req, _gfp_flags);

	req = container_of(_req, dwc_otg_pcd_request_t, req);
	if (!_req || !_req->complete || !_req->buf ||
            !list_empty(&req->queue)) {
		DWC_WARN("%s, bad params\n", __func__);
		return -EINVAL;
	}

	ep = container_of(_ep, dwc_otg_pcd_ep_t, ep);
	if (!_ep || (!ep->desc && ep->dwc_ep.num != 0)) {
		DWC_WARN("%s, bad ep\n", __func__);
		return -EINVAL;
	}
	pcd = ep->pcd;
        if (!pcd->driver || pcd->gadget.speed == USB_SPEED_UNKNOWN) {
                DWC_DEBUGPL(DBG_PCDV, "gadget.speed=%d\n", pcd->gadget.speed);
                DWC_WARN("%s, bogus device state\n", __func__);
		return -ESHUTDOWN;
	}

	DWC_DEBUGPL(DBG_PCD, "%s queue req %p, len %d buf %p\n",
                   _ep->name, _req, _req->length, _req->buf);

	SPIN_LOCK_IRQSAVE(&ep->pcd->lock, flags);

#if defined(DEBUG) & defined(VERBOSE)
        dump_msg(_req->buf, _req->length);
#endif

	_req->status = -EINPROGRESS;
	_req->actual = 0;

	/* DFX TODO remove debug aid. */
	/*if ( ep->dwc_ep.num != 0 ) {
		if ( ep->dwc_ep.is_in )
			in_count++;
		else
			out_count++;

		if ( (in_count % 100 == 0) || (out_count % 100 == 0) )  {
			printk(KERN_ERR "DFX ep Q ep num: %d is_in: %d active: %d "
					"ep stopped: %d req len: 0x%x\n",
			       ep->dwc_ep.num, ep->dwc_ep.is_in, ep->dwc_ep.active,
			       ep->stopped, _req->length);
		}
        }
	*/

	/* DFX TODO What do they mean by this statement? */
	/*
         * For EP0 IN without premature status, zlp is required?
	 */
	if (ep->dwc_ep.num == 0 && ep->dwc_ep.is_in) {
                DWC_DEBUGPL(DBG_PCDV, "%s-OUT ZLP\n", _ep->name);
		//_req->zero = 1;
        }

        /* Start the transfer. */
	if (list_empty(&ep->queue) && !ep->stopped) {
                /* EP0 Transfer? */
                if (ep->dwc_ep.num == 0) {
			switch (pcd->ep0state) {
			case EP0_IN_DATA_PHASE:
				DWC_DEBUGPL(DBG_PCD,
                                            "%s ep0: EP0_IN_DATA_PHASE\n",
                                            __func__);
				break;

			case EP0_OUT_DATA_PHASE:
				DWC_DEBUGPL(DBG_PCD,
                                            "%s ep0: EP0_OUT_DATA_PHASE\n",
                                            __func__);
                                if (pcd->request_config) {
                                /* Complete STATUS PHASE for SET_CONFIGURATION
				 * Control Transfer.  There is no data phase, so
				 * at this point, need to send ZLP to host.
				 */
				/* DFX TODO this obviously needs to be fixed. */
					int top = dwc_otg_top_nptxfifo_epnum(GET_CORE_IF(pcd));
					if ( top != NP_TXFIFO_EMPTY && top != 0 )  {
						dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
						//dwc_otg_core_global_regs_t *global_regs =
						//	core_if->core_global_regs;
						//printk(KERN_ERR "DFX SET_CONFIG NP tx Q top ep NOT 0. "
						//		"txstatus: 0x%08x\n", dwc_read_reg32(&global_regs->gnptxsts));
						dwc_otg_flush_tx_fifo(core_if, 0);
						pcd->next_ep = 0;
					}

                                        /* original code */
					ep->dwc_ep.is_in = 1;
                                        pcd->ep0state = EP0_STATUS;
                                }
				break;

                        default:
				DWC_DEBUGPL(DBG_ANY, "ep0: odd state %d\n",
                                            pcd->ep0state);
                                SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);
				return -EL2HLT;
                        }

			ep->dwc_ep.dma_addr = _req->dma;
                        ep->dwc_ep.start_xfer_buff = _req->buf;
                        ep->dwc_ep.xfer_buff = _req->buf;
                        ep->dwc_ep.xfer_len = _req->length;
                        ep->dwc_ep.xfer_count = 0;
                        ep->dwc_ep.sent_zlp = 0;
                        ep->dwc_ep.total_len = ep->dwc_ep.xfer_len;
                        dwc_otg_ep0_start_transfer( GET_CORE_IF(pcd),
                                                    &ep->dwc_ep );

                } else {
                        /* Setup and start the Transfer */
			ep->dwc_ep.dma_addr = _req->dma;
                        ep->dwc_ep.start_xfer_buff = _req->buf;
                        ep->dwc_ep.xfer_buff = _req->buf;
                        ep->dwc_ep.xfer_len = _req->length;
                        ep->dwc_ep.xfer_count = 0;
                        ep->dwc_ep.sent_zlp = 0;
                        ep->dwc_ep.total_len = ep->dwc_ep.xfer_len;
                        dwc_otg_ep_start_transfer( GET_CORE_IF(pcd),
                                                   &ep->dwc_ep );
                }
        }

	/* DFX TODO Comment in request completion function says: don't modify
	 * queue heads during completion callback.  Why then isn't that
	 * checked for here?  Because only the tail is being modified.
	 */
	/*
	if ( req != 0 && ep->stopped )  {
		printk(KERN_ERR "dwc_otg: request queue modified when ep %d "
				"stopped\n", ep->dwc_ep.num);
	}
	*/
	if (req != 0) {
                ++pcd->request_pending;
		list_add_tail(&req->queue, &ep->queue);
		/* DFX TODO check logic for this.  When to enable this
		 * interrupt? Use for not ep0 only?
		 *
		 * DFX TODO WHY IS this coded as .. && ep->stopped.  That does
		 * not make sense, should be && !ep->stopped?  When tried,
		 * enumeration broke again.  But maybe just because a handler is
		 * not written for it?
		 */
                if (ep->dwc_ep.is_in && !ep->stopped && ep->dwc_ep.num != 0 && !(GET_CORE_IF(pcd)->dma_enable)) {
                        /** @todo NGS Create a function for this. */
                        diepmsk_data_t diepmsk = { .d32 = 0};
                        diepmsk.b.intktxfemp = 1;
                        dwc_modify_reg32( &GET_CORE_IF(pcd)->dev_if->dev_global_regs->diepmsk, 0, diepmsk.d32 );
                }
        }

        SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);
        return 0;
}

/**
 * This function cancels an I/O request from an EP.
 */
static int dwc_otg_pcd_ep_dequeue(struct usb_ep *_ep,
                                  struct usb_request *_req)
{
	dwc_otg_pcd_request_t *req;
	dwc_otg_pcd_ep_t *ep;
	dwc_otg_pcd_t	*pcd;
	unsigned long flags;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p,%p)\n", __func__, _ep, _req);

	ep = container_of(_ep, dwc_otg_pcd_ep_t, ep);
	if (!_ep || !_req || (!ep->desc && ep->dwc_ep.num != 0)) {
		DWC_WARN("%s, bad argument\n", __func__);
		return -EINVAL;
	}
	pcd = ep->pcd;
        if (!pcd->driver || pcd->gadget.speed == USB_SPEED_UNKNOWN) {
                DWC_WARN("%s, bogus device state\n", __func__);
		return -ESHUTDOWN;
	}

	SPIN_LOCK_IRQSAVE(&pcd->lock, flags);
        DWC_DEBUGPL(DBG_PCDV, "%s %s %s %p\n", __func__, _ep->name,
                    ep->dwc_ep.is_in ? "IN" : "OUT",
                    _req);

	/* make sure it's actually queued on this endpoint */
	list_for_each_entry( req, &ep->queue, queue) {
		if (&req->req == _req) {
			break;
                }
	}

	if (&req->req != _req) {
                SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);
		return -EINVAL;
	}

        if (!list_empty(&req->queue)) {
		request_done(ep, req, -ECONNRESET);
        } else {
		req = 0;
        }

        SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);

	return req ? 0 : -EOPNOTSUPP;
}

/**
 * usb_ep_set_halt stalls an endpoint.
 *
 * usb_ep_clear_halt clears an endpoint halt and resets its data
 * toggle.
 *
 * Both of these functions are implemented with the same underlying
 * function. The behavior depends on the value argument.
 *
 * @param[in] _ep the Endpoint to halt or clear halt.
 * @param[in] _value
 *	- 1 means set_halt,
 *	- 0 means clear_halt.
 */
static int dwc_otg_pcd_ep_set_halt(struct usb_ep *_ep, int _value)
{
        int retval = 0;
	unsigned long flags;
        dwc_otg_pcd_ep_t *ep = 0;


        DWC_DEBUGPL(DBG_PCD,"HALT %s %d\n", _ep->name, _value);

        ep = container_of(_ep, dwc_otg_pcd_ep_t, ep);
	if (!_ep || (!ep->desc && ep != &ep->pcd->ep[0]) ||
            ep->desc->bmAttributes == USB_ENDPOINT_XFER_ISOC) {
		DWC_WARN("%s, bad ep\n", __func__);
		return -EINVAL;
        }

	SPIN_LOCK_IRQSAVE(&ep->pcd->lock, flags);
	if (!list_empty(&ep->queue)){
                DWC_WARN("%s() %s XFer In process\n", __func__, _ep->name);
		retval = -EAGAIN;
        }
        else if (_value == 0) {
                dwc_otg_ep_clear_stall( ep->pcd->otg_dev->core_if,
                                        &ep->dwc_ep );
        } else {
                if (ep->dwc_ep.num == 0) {
                        ep->pcd->ep0state = EP0_STALL;
                }
                ep->stopped = 1;
                dwc_otg_ep_set_stall( ep->pcd->otg_dev->core_if,
                                      &ep->dwc_ep );
        }
	SPIN_UNLOCK_IRQRESTORE(&ep->pcd->lock, flags);
        return retval;
}


static struct usb_ep_ops dwc_otg_pcd_ep_ops = {
	.enable		= dwc_otg_pcd_ep_enable,
	.disable	= dwc_otg_pcd_ep_disable,

	.alloc_request	= dwc_otg_pcd_alloc_request,
	.free_request	= dwc_otg_pcd_free_request,

	.queue		= dwc_otg_pcd_ep_queue,
	.dequeue	= dwc_otg_pcd_ep_dequeue,

	.set_halt	= dwc_otg_pcd_ep_set_halt,
	.fifo_status	= 0,
	.fifo_flush	= 0,
};

/*  Gadget Operations */
/**
 * The following gadget operations will be implemented in the DWC_otg
 * PCD. Functions in the API that are not described below are not
 * implemented.
 *
 * The Gadget API provides wrapper functions for each of the function
 * pointers defined in usb_gadget_ops. The Gadget Driver calls the
 * wrapper function, which then calls the underlying PCD function. The
 * following sections are named according to the wrapper functions
 * (except for ioctl, which doesn't have a wrapper function). Within
 * each section, the corresponding DWC_otg PCD function name is
 * specified.
 *
 */

/**
 *Gets the USB Frame number of the last SOF.
 */
static int dwc_otg_pcd_get_frame(struct usb_gadget *_gadget)
{
        dwc_otg_pcd_t *pcd;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n", __func__, _gadget);

        if (_gadget == 0){
                return -ENODEV;
        } else {
                pcd = container_of(_gadget, dwc_otg_pcd_t, gadget);
                dwc_otg_get_frame_number( GET_CORE_IF(pcd) );
        }

        return 0;
}

void dwc_otg_pcd_initiate_srp(dwc_otg_pcd_t *_pcd)
{
	uint32_t *addr = (uint32_t *)&(GET_CORE_IF(_pcd)->core_global_regs->gotgctl);
	gotgctl_data_t mem;
	gotgctl_data_t val;

	val.d32 = dwc_read_reg32( addr );
        if (val.b.sesreq) {
        	DWC_ERROR("Session Request Already active!\n");
                return;
        }

        DWC_NOTICE("Session Request Initated\n");
	mem.d32 = dwc_read_reg32(addr);
	mem.b.sesreq = 1;
	dwc_write_reg32(addr, mem.d32);

        /* Start the SRP timer */
        dwc_otg_pcd_start_srp_timer( _pcd );
	return;
}

void dwc_otg_pcd_remote_wakeup(dwc_otg_pcd_t *_pcd, int set)
{
	dctl_data_t dctl = {.d32=0};
	volatile uint32_t *addr =
                &(GET_CORE_IF(_pcd)->dev_if->dev_global_regs->dctl);

        if (dwc_otg_is_device_mode(GET_CORE_IF(_pcd))) {
                if (_pcd->remote_wakeup_enable) {
			if (set) {
				dctl.b.rmtwkupsig = 1;
				dwc_modify_reg32( addr, 0, dctl.d32 );
				DWC_DEBUGPL(DBG_PCD, "Set Remote Wakeup\n");
				mdelay(1);
				dwc_modify_reg32( addr, dctl.d32, 0 );
				DWC_DEBUGPL(DBG_PCD, "Clear Remote Wakeup\n");
			}
			else {
			}
		}
		else {
                        DWC_DEBUGPL(DBG_PCD, "Remote Wakeup is disabled\n");
                }
        }

	return;
}

/**
 * Initiates Session Request Protocol (SRP) to wakeup the host if no
 * session is in progress. If a session is already in progress, but
 * the device is suspended, remote wakeup signaling is started.
 *
 */
static int dwc_otg_pcd_wakeup(struct usb_gadget *_gadget)
{
	unsigned long flags;
        dwc_otg_pcd_t *pcd;
	dsts_data_t     dsts;
	gotgctl_data_t  gotgctl;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n", __func__, _gadget);

        if (_gadget == 0){
                return -ENODEV;
        } else {
                pcd = container_of(_gadget, dwc_otg_pcd_t, gadget);
        }
	SPIN_LOCK_IRQSAVE(&pcd->lock, flags);

	/*
	 * This function starts the Protocol if no session is in progress. If
	 * a session is already in progress, but the device is suspended,
	 * remote wakeup signaling is started.
	 */

	/* Check if valid session */
        gotgctl.d32 = dwc_read_reg32(&(GET_CORE_IF(pcd)->core_global_regs->gotgctl));
	if (gotgctl.b.bsesvld) {
		/* Check if suspend state */
		dsts.d32 = dwc_read_reg32(&(GET_CORE_IF(pcd)->dev_if->dev_global_regs->dsts));
		if (dsts.b.suspsts) {
			dwc_otg_pcd_remote_wakeup(pcd, 1);
		}
	}
	else {
		dwc_otg_pcd_initiate_srp(pcd);
	}

	SPIN_UNLOCK_IRQRESTORE(&pcd->lock, flags);
        return 0;
}

static const struct usb_gadget_ops dwc_otg_pcd_ops = {
	.get_frame	 = dwc_otg_pcd_get_frame,
	.wakeup		 = dwc_otg_pcd_wakeup,
	// current versions must always be self-powered
};

/**
 * This function updates the otg values in the gadget structure.
 */
void dwc_otg_pcd_update_otg( dwc_otg_pcd_t *_pcd, const unsigned _reset )
{

	if (!_pcd->gadget.is_otg)
		return;

        if (_reset) {
                _pcd->b_hnp_enable = 0;
                _pcd->a_hnp_support = 0;
                _pcd->a_alt_hnp_support = 0;
        }

	_pcd->gadget.b_hnp_enable = _pcd->b_hnp_enable;
	_pcd->gadget.a_hnp_support =  _pcd->a_hnp_support;
	_pcd->gadget.a_alt_hnp_support = _pcd->a_alt_hnp_support;
}

/**
 * This function is the top level PCD interrupt handler.
 */
static irqreturn_t
dwc_otg_pcd_irq(int _irq, void *_dev)
{
        dwc_otg_pcd_t *pcd = _dev;
        int32_t retval = IRQ_NONE;

        retval = dwc_otg_pcd_handle_intr( pcd );
	return IRQ_RETVAL(retval);
}

/**
 * PCD Callback function for initializing the PCD when switching to
 * device mode.
 *
 * @param _p void pointer to the <code>dwc_otg_pcd_t</code>
 */
static int32_t dwc_otg_pcd_start_cb( void *_p )
{
        dwc_otg_pcd_t *pcd = (dwc_otg_pcd_t *)_p;
        /*
         * Initialized the Core for Device mode.
         */
        if (dwc_otg_is_device_mode( GET_CORE_IF(pcd) )){
                dwc_otg_core_dev_init(GET_CORE_IF(pcd));
        }
        return 1;
}

/**
 * PCD Callback function for stopping the PCD when switching to Host
 * mode.
 *
 * @param _p void pointer to the <code>dwc_otg_pcd_t</code>
 */
static int32_t dwc_otg_pcd_stop_cb( void *_p )
{
        dwc_otg_pcd_t *pcd = (dwc_otg_pcd_t *)_p;
        extern void dwc_otg_pcd_stop(dwc_otg_pcd_t *_pcd);

        dwc_otg_pcd_stop( pcd );
        return 1;
}


/**
 * PCD Callback function for notifying the PCD when resuming from
 * suspend.
 *
 * @param _p void pointer to the <code>dwc_otg_pcd_t</code>
 */
static int32_t dwc_otg_pcd_suspend_cb( void *_p )
{
        dwc_otg_pcd_t *pcd = (dwc_otg_pcd_t *)_p;
        if (pcd->driver && pcd->driver->resume) {
                SPIN_UNLOCK(&pcd->lock);
                pcd->driver->suspend(&pcd->gadget);
                SPIN_LOCK(&pcd->lock);
        }

        return 1;
}


/**
 * PCD Callback function for notifying the PCD when resuming from
 * suspend.
 *
 * @param _p void pointer to the <code>dwc_otg_pcd_t</code>
 */
static int32_t dwc_otg_pcd_resume_cb( void *_p )
{
        dwc_otg_pcd_t *pcd = (dwc_otg_pcd_t *)_p;
        if (pcd->driver && pcd->driver->resume) {
                SPIN_UNLOCK(&pcd->lock);
                pcd->driver->resume(&pcd->gadget);
                SPIN_LOCK(&pcd->lock);
        }
        /* Stop the SRP timeout timer. */
	if ((GET_CORE_IF(pcd)->core_params->phy_type != DWC_PHY_TYPE_PARAM_FS) ||
	    (!GET_CORE_IF(pcd)->core_params->i2c_enable))
	{
		if (GET_CORE_IF(pcd)->srp_timer_started) {
			GET_CORE_IF(pcd)->srp_timer_started = 0;
			del_timer( &pcd->srp_timer );
		}
	}
        return 1;
}


/**
 * PCD Callback structure for handling mode switching.
 */
static dwc_otg_cil_callbacks_t pcd_callbacks = {
        .start = dwc_otg_pcd_start_cb,
        .stop = dwc_otg_pcd_stop_cb,
        .suspend = dwc_otg_pcd_suspend_cb,
        .resume_wakeup = dwc_otg_pcd_resume_cb,
        .p = 0, /* Set at registration */
};

/**
 * This function is called when the SRP timer expires.  The SRP should
 * complete within 6 seconds.
 */
static void srp_timeout( unsigned long _ptr )
{
	gotgctl_data_t gotgctl;
        dwc_otg_core_if_t *core_if = (dwc_otg_core_if_t *)_ptr;
	volatile uint32_t *addr = &core_if->core_global_regs->gotgctl;

	gotgctl.d32 = dwc_read_reg32(addr);

	core_if->srp_timer_started = 0;

	if ((core_if->core_params->phy_type == DWC_PHY_TYPE_PARAM_FS) &&
	    (core_if->core_params->i2c_enable))
	{
                DWC_PRINT( "SRP Timeout\n");

		if ((core_if->srp_success) &&
		    (gotgctl.b.bsesvld))
		{
			if (core_if->pcd_cb && core_if->pcd_cb->resume_wakeup ) {
				core_if->pcd_cb->resume_wakeup(core_if->pcd_cb->p);
			}
			/* Clear Session Request */
			gotgctl.d32 = 0;
			gotgctl.b.sesreq = 1;
			dwc_modify_reg32( &core_if->core_global_regs->gotgctl,
					  gotgctl.d32, 0);

			core_if->srp_success = 0;
		}
		else {
			DWC_ERROR( "Device not connected/responding\n");
			gotgctl.b.sesreq = 0;
			dwc_write_reg32(addr, gotgctl.d32);
		}
	}
        else if (gotgctl.b.sesreq)
	{
                DWC_PRINT( "SRP Timeout\n");

		DWC_ERROR( "Device not connected/responding\n");
		gotgctl.b.sesreq = 0;
		dwc_write_reg32(addr, gotgctl.d32);
        } else {
                DWC_PRINT( " SRP GOTGCTL=%0x\n", gotgctl.d32);
        }
}

/**
 * Start the SRP timer to detect when the SRP does not complete within
 * 6 seconds.
 *
 * @param _pcd the pcd structure.
 */
void dwc_otg_pcd_start_srp_timer(dwc_otg_pcd_t *_pcd )
{
        struct timer_list *srp_timer = &_pcd->srp_timer;
	GET_CORE_IF(_pcd)->srp_timer_started = 1;
        init_timer( srp_timer );
        srp_timer->function = srp_timeout;
        srp_timer->data = (unsigned long)GET_CORE_IF(_pcd);
        srp_timer->expires = jiffies + (HZ*6);
        add_timer( srp_timer );
}

/**
 * Tasklet
 *
 */
extern void start_next_request( dwc_otg_pcd_ep_t *_ep );

static void start_xfer_tasklet_func (unsigned long data)
{
	dwc_otg_pcd_t *pcd = (dwc_otg_pcd_t*)data;
	dwc_otg_core_if_t *core_if = pcd->otg_dev->core_if;

	int i;

	DWC_DEBUGPL(DBG_PCDV, "Start xfer tasklet\n");

        for (i=0; i<core_if->dev_if->num_eps; i++) {

		depctl_data_t diepctl;
                diepctl.d32 = dwc_read_reg32( &core_if->dev_if->in_ep_regs[i]->diepctl);

		if (pcd->ep[i].queue_sof) {
			pcd->ep[i].queue_sof = 0;
			start_next_request (&pcd->ep[i]);
		}

	}

	return;
}

static struct tasklet_struct start_xfer_tasklet = {
	.next = NULL,
	.state = 0,
	.count = ATOMIC_INIT(0),
	.func = start_xfer_tasklet_func,
	.data = 0,
};


/**
 * This function initialized the pcd Dp structures to there default
 * state.
 *
 * @param _pcd the pcd structure.
 */
void dwc_otg_pcd_reinit(dwc_otg_pcd_t *_pcd)
{
        static const char * names[] = {
                "ep0", "ep1", "ep2", "ep3",
                "ep4", "ep5", "ep6", "ep7",
                "ep8", "ep9", "ep10", "ep11",
                "ep12", "ep13", "ep14", "ep15",
        };
        uint32_t i;

        DWC_DEBUGPL(DBG_PCDV, "%s(%p)\n", __func__, _pcd);

	INIT_LIST_HEAD (&_pcd->gadget.ep_list);
	_pcd->gadget.ep0 = &_pcd->ep[0].ep;
	_pcd->gadget.speed = USB_SPEED_UNKNOWN;

	INIT_LIST_HEAD (&_pcd->gadget.ep0->ep_list);

        /**
         * Initialize the EP structures.
         */
	for (i = 0; i < _pcd->num_eps; i++) {
                dwc_otg_pcd_ep_t *ep = &_pcd->ep[i];

                /* Init EP structure */
                ep->desc = 0;
                ep->pcd = _pcd;
                ep->stopped = 1;

                /* Init DWC ep structure */
                ep->dwc_ep.num = i;
                ep->dwc_ep.active = 0;
                ep->dwc_ep.tx_fifo_num = 0;
                /* Control until ep is actvated */
                ep->dwc_ep.type = DWC_OTG_EP_TYPE_CONTROL;
                ep->dwc_ep.maxpacket = MAX_PACKET_SIZE;
                ep->dwc_ep.dma_addr = 0;
                ep->dwc_ep.start_xfer_buff = 0;
                ep->dwc_ep.xfer_buff = 0;
                ep->dwc_ep.xfer_len = 0;
                ep->dwc_ep.xfer_count = 0;
		ep->dwc_ep.sent_zlp = 0;
		ep->dwc_ep.total_len = 0;

                /* Init the usb_ep structure. */
                /**
                 * @todo NGS: Add direction to EP, based on contents
                 * of HWCFG1.  Need a copy of HWCFG1 in pcd structure?
                 * sprintf( ";r
                 */
		ep->ep.name = names[i];
		ep->ep.ops = &dwc_otg_pcd_ep_ops;
                /**
                 * @todo NGS: What should the max packet size be set to
                 * here?  Before EP type is set?
                 */
                ep->ep.maxpacket = MAX_PACKET_SIZE;

                list_add_tail (&ep->ep.ep_list, &_pcd->gadget.ep_list);

                INIT_LIST_HEAD (&ep->queue);
        }

	/* remove ep0 from the list.  There is a ep0 pointer.*/
	list_del_init (&_pcd->ep[0].ep.ep_list);

	_pcd->ep0state = EP0_DISCONNECT;
	_pcd->ep[0].ep.maxpacket = MAX_EP0_SIZE;
        _pcd->ep[0].dwc_ep.maxpacket = MAX_EP0_SIZE;
        _pcd->ep[0].dwc_ep.type = DWC_OTG_EP_TYPE_CONTROL;
}

/**
 * This function releases the Gadget device.
 * required by device_unregister().
 *
 * @todo Should this do something?  Should it free the PCD?
 */
static void dwc_otg_pcd_gadget_release(struct device *_dev)
{
	DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n", __func__, _dev);
}

/**
 * This function initializes the PCD portion of the driver.
 *
 */
int __init dwc_otg_pcd_init(struct device *_dev)
{
        static char pcd_name[] = "dwc_otg_pcd";
        dwc_otg_pcd_t *pcd;
        dwc_otg_device_t *otg_dev = dev_get_drvdata(_dev);
        int retval = 0;

        DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n",__func__, _dev );
        /*
         * Allocate PCD structure
         */
        pcd = kmalloc( sizeof(dwc_otg_pcd_t), GFP_KERNEL);
        if (pcd == 0) {
                return -ENOMEM;
        }
        memset( pcd, 0, sizeof(dwc_otg_pcd_t));
        spin_lock_init( &pcd->lock );

        otg_dev->pcd = pcd;
        s_pcd = pcd;
        pcd->gadget.name = pcd_name;
        strcpy(pcd->gadget.dev.bus_id, "gadget");

        pcd->otg_dev = dev_get_drvdata(_dev);


        pcd->gadget.dev.parent = _dev;
	pcd->gadget.dev.release = dwc_otg_pcd_gadget_release;
        pcd->gadget.ops = &dwc_otg_pcd_ops;

	/* If the module is set to FS or if the PHY_TYPE is FS then the gadget
	 * should not report as dual-speed capable.  If is_dualspeed = 0 then
	 * the gadget driver should not report a device qualifier descriptor
	 * when queried.
	 */
	if (GET_CORE_IF(pcd)->core_params->speed == DWC_SPEED_PARAM_FULL) {
		pcd->gadget.is_dualspeed = 0;
	}
	else {
		pcd->gadget.is_dualspeed = 1;
	}

        if (GET_CORE_IF(pcd)->core_params->otg_cap !=
            DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE)
	{
                pcd->gadget.is_otg = 1;
        }

        pcd->driver = 0;
        /* Register the gadget device */
	retval = device_register( &pcd->gadget.dev );
	if (retval) {
                DWC_ERROR("device_register failed with %d\n", retval);
		return -ENODEV;
	}

        /*
         * Initialized the Core for Device mode.
         */
        if (dwc_otg_is_device_mode( GET_CORE_IF(pcd) )) {
                dwc_otg_core_dev_init( GET_CORE_IF(pcd) );
        }

        /*
         * Initialize EP structures
	 *
	 * DFX Note:  The original driver initialized pcd->num_eps to the value
	 * of num_dev_ep in the hardware configuration 2 register.  But, this
	 * value is the number of end points *in addition to* end point 0.  The
	 * original driver seems to be assuming it is the *total* number of end
	 * points, which leads to one too few end points on Ultra.
         */
        pcd->num_eps = otg_dev->core_if->hwcfg2.b.num_dev_ep + 1;
        dwc_otg_pcd_reinit( pcd );

        /*
         * Register the PCD Callbacks.
         */
        dwc_otg_cil_register_pcd_callbacks( otg_dev->core_if, &pcd_callbacks,
                                            pcd );
        /*
         * Setup interupt handler
         */
        DWC_DEBUGPL( DBG_ANY, "registering handler for irq%d\n", otg_dev->irq);
	retval = request_irq(otg_dev->irq, dwc_otg_pcd_irq,
                             IRQF_SHARED, pcd->gadget.name, pcd);
        if (retval != 0) {
                DWC_ERROR("request of irq%d failed\n", otg_dev->irq);
		kfree (pcd);
                return -EBUSY;
        }

	/*
	 * Initialize the DMA buffer for SETUP packets
	 */
	if (GET_CORE_IF(pcd)->dma_enable) {
		pcd->setup_pkt = dma_alloc_coherent (NULL, sizeof (*pcd->setup_pkt) * 5, &pcd->setup_pkt_dma_handle, 0);
		pcd->status_buf = dma_alloc_coherent (NULL, sizeof (uint16_t), &pcd->status_buf_dma_handle, 0);
	}
	else {
		pcd->setup_pkt = kmalloc (sizeof (*pcd->setup_pkt) * 5, GFP_KERNEL);
		pcd->status_buf = kmalloc (sizeof (uint16_t), GFP_KERNEL);
	}

	if (pcd->setup_pkt == 0) {
		kfree (pcd);
		return -ENOMEM;
	}

	/* Initialize tasklet */
	start_xfer_tasklet.data = (unsigned long)pcd;
	pcd->start_xfer_tasklet = &start_xfer_tasklet;

        return 0;
}

/**
 * Cleanup the PCD.
 */
void dwc_otg_pcd_remove( struct device *_dev )
{
        dwc_otg_device_t *otg_dev = dev_get_drvdata(_dev);
        dwc_otg_pcd_t *pcd = otg_dev->pcd;

        DWC_DEBUGPL(DBG_PCDV, "%s(%p)\n", __func__, _dev);

        /*
         * Free the IRQ
         */
        free_irq( otg_dev->irq, pcd );

         /* start with the driver above us */
        if (pcd->driver) {
                /* should have been done already by driver model core */
                DWC_WARN("driver '%s' is still registered\n",
                         pcd->driver->driver.name);
                usb_gadget_unregister_driver( pcd->driver);
        }
        device_unregister(&pcd->gadget.dev);

	if (GET_CORE_IF(pcd)->dma_enable) {
		dma_free_coherent (NULL, sizeof (*pcd->setup_pkt) * 5, pcd->setup_pkt, pcd->setup_pkt_dma_handle);
		dma_free_coherent (NULL, sizeof (uint16_t), pcd->status_buf, pcd->status_buf_dma_handle);
	}
	else {
		kfree (pcd->setup_pkt);
		kfree (pcd->status_buf);
	}

        kfree( pcd );
        otg_dev->pcd = 0;
}

/**
 * This function registers a gadget driver with the PCD.
 *
 * When a driver is successfully registered, it will receive control
 * requests including set_configuration(), which enables non-control
 * requests.  then usb traffic follows until a disconnect is reported.
 * then a host may connect again, or the driver might get unbound.
 *
 * @param _driver The driver being registered
 */
int usb_gadget_register_driver(struct usb_gadget_driver *_driver)
{
        int retval;

	DWC_DEBUGPL(DBG_PCD, "registering gadget driver '%s'\n", _driver->driver.name);

	if (!_driver || _driver->speed == USB_SPEED_UNKNOWN ||
            !_driver->bind ||
            !_driver->disconnect ||
            !_driver->setup) {
                DWC_DEBUGPL(DBG_PCDV,"EINVAL\n");
		return -EINVAL;
        }
	if (s_pcd == 0) {
                DWC_DEBUGPL(DBG_PCDV,"ENODEV\n");
		return -ENODEV;
        }
	if (s_pcd->driver != 0) {
                DWC_DEBUGPL(DBG_PCDV,"EBUSY (%p)\n", s_pcd->driver);
		return -EBUSY;
        }

	/* hook up the driver */
	s_pcd->driver = _driver;
	s_pcd->gadget.dev.driver = &_driver->driver;

        DWC_DEBUGPL(DBG_PCD, "bind to driver %s\n", _driver->driver.name);
	retval = _driver->bind(&s_pcd->gadget);
	if (retval) {
		DWC_ERROR("bind to driver %s --> error %d\n",
                           _driver->driver.name, retval);
		s_pcd->driver = 0;
		s_pcd->gadget.dev.driver = 0;
		return retval;
	}
	DWC_DEBUGPL(DBG_ANY, "registered gadget driver '%s'\n",
                    _driver->driver.name);
	return 0;
}
EXPORT_SYMBOL(usb_gadget_register_driver);

/**
 * This function unregisters a gadget driver
 *
 * @param _driver The driver being unregistered
 */
int usb_gadget_unregister_driver(struct usb_gadget_driver *_driver)
{
	//DWC_DEBUGPL(DBG_PCDV,"%s(%p)\n", __func__, _driver);

        if (s_pcd == 0) {
                DWC_DEBUGPL(DBG_ANY, "%s Return(%d): s_pcd==0\n", __func__,
                            -ENODEV);
                return -ENODEV;
        }
        if (_driver == 0 || _driver != s_pcd->driver) {
                DWC_DEBUGPL( DBG_ANY, "%s Return(%d): driver?\n", __func__,
                            -EINVAL);
                return -EINVAL;
        }

	if (_driver->unbind)
		_driver->unbind(&s_pcd->gadget);
        s_pcd->driver = 0;

	DWC_DEBUGPL(DBG_ANY, "unregistered driver '%s'\n",
                    _driver->driver.name);
	return 0;
}
EXPORT_SYMBOL(usb_gadget_unregister_driver);

#endif /* DWC_HOST_ONLY */
