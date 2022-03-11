cmd_drivers/mtd/mtd_blkdevs.o := ppc_4xxFP-gcc -m32 -Wp,-MD,drivers/mtd/.mtd_blkdevs.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mtd_blkdevs)"  -D"KBUILD_MODNAME=KBUILD_STR(mtd_blkdevs)" -c -o drivers/mtd/mtd_blkdevs.o drivers/mtd/mtd_blkdevs.c

deps_drivers/mtd/mtd_blkdevs.o := \
  drivers/mtd/mtd_blkdevs.c \
  include/linux/kernel.h \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/numa.h) \
  /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include/stdarg.h \
  include/linux/linkage.h \
  arch/ppc/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/forced/inlining.h) \
  include/linux/compiler-gcc.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lsf.h) \
    $(wildcard include/config/resources/64bit.h) \
  include/linux/posix_types.h \
  arch/ppc/include/asm/posix_types.h \
  arch/ppc/include/asm/types.h \
  include/linux/bitops.h \
  arch/ppc/include/asm/bitops.h \
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  arch/ppc/include/asm/synch.h \
    $(wildcard include/config/smp.h) \
  include/linux/stringify.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/sched.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  arch/ppc/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  arch/ppc/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slabinfo.h) \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/highmem.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
  arch/ppc/include/asm/thread_info.h \
    $(wildcard include/config/ppc/page/256k.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/cache.h \
  arch/ppc/include/asm/cache.h \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/403gcx.h) \
    $(wildcard include/config/ppc32.h) \
  arch/ppc/include/asm/processor.h \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
  arch/ppc/include/asm/reg.h \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/apus/fast/except.h) \
    $(wildcard include/config/ppc/cell.h) \
  arch/ppc/include/asm/cputable.h \
    $(wildcard include/config/mpc10x/bridge.h) \
    $(wildcard include/config/ppc/83xx.h) \
    $(wildcard include/config/8260.h) \
    $(wildcard include/config/bdi/switch.h) \
    $(wildcard include/config/power3.h) \
    $(wildcard include/config/power4.h) \
    $(wildcard include/config/ppc/pasemi/a2/workarounds.h) \
    $(wildcard include/config/44x.h) \
    $(wildcard include/config/e200.h) \
    $(wildcard include/config/e500.h) \
  include/asm/reg_booke.h \
    $(wildcard include/config/440a.h) \
  arch/ppc/include/asm/ptrace.h \
  include/asm/page.h \
    $(wildcard include/config/ppc/page/4k.h) \
    $(wildcard include/config/ppc/page/16k.h) \
    $(wildcard include/config/ppc/page/64k.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/pte/64bit.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/asm/system.h \
    $(wildcard include/config/6xx.h) \
    $(wildcard include/config/booke/wdt.h) \
  arch/ppc/include/asm/hw_irq.h \
  include/linux/errno.h \
  arch/ppc/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/locking.h) \
  include/linux/spinlock_up.h \
  include/linux/spinlock_api_up.h \
  arch/ppc/include/asm/atomic.h \
  include/asm-generic/atomic.h \
  include/linux/wait.h \
  arch/ppc/include/asm/current.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/linux/seqlock.h \
  include/linux/nodemask.h \
  include/linux/bitmap.h \
  include/linux/string.h \
  arch/ppc/include/asm/string.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  arch/ppc/include/asm/rwsem.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/cpumask.h \
  include/linux/smp.h \
  arch/ppc/include/asm/topology.h \
    $(wildcard include/config/pci.h) \
  include/asm-generic/topology.h \
  include/linux/slab_def.h \
  include/linux/kmalloc_sizes.h \
  include/linux/module.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/stat.h \
  arch/ppc/include/asm/stat.h \
  include/linux/time.h \
  include/linux/kmod.h \
    $(wildcard include/config/kmod.h) \
  include/linux/elf.h \
  include/linux/elf-em.h \
  arch/ppc/include/asm/elf.h \
    $(wildcard include/config/spu/base.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/fair/user/sched.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/fault/injection.h) \
  arch/ppc/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/capability.h \
  include/linux/timex.h \
  arch/ppc/include/asm/timex.h \
  include/linux/jiffies.h \
  include/linux/calc64.h \
  arch/ppc/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/rbtree.h \
  include/linux/mm_types.h \
    $(wildcard include/config/mmu.h) \
  include/linux/auxvec.h \
  arch/ppc/include/asm/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/completion.h \
  include/asm/mmu.h \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/serial/text/debug.h) \
  arch/ppc/include/asm/semaphore.h \
  arch/ppc/include/asm/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
  arch/ppc/include/asm/ipcbuf.h \
  include/linux/kref.h \
  arch/ppc/include/asm/sembuf.h \
  include/linux/signal.h \
  arch/ppc/include/asm/signal.h \
  include/asm-generic/signal.h \
  arch/ppc/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/securebits.h \
  include/linux/fs_struct.h \
  include/linux/pid.h \
  include/linux/rcupdate.h \
  include/linux/percpu.h \
  arch/ppc/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  arch/ppc/include/asm/seccomp.h \
  include/linux/unistd.h \
  arch/ppc/include/asm/unistd.h \
  include/linux/futex.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/param.h \
  include/linux/resource.h \
  arch/ppc/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/aio.h \
  include/linux/workqueue.h \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/moduleparam.h \
  include/linux/marker.h \
  arch/ppc/include/asm/local.h \
  arch/ppc/include/asm/module.h \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  arch/ppc/include/asm/ioctl.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
    $(wildcard include/config/profiling.h) \
  include/linux/namei.h \
  include/linux/radix-tree.h \
  include/linux/quota.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/fcntl.h \
  arch/ppc/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
    $(wildcard include/config/64bit.h) \
  include/linux/err.h \
  include/linux/mtd/blktrans.h \
  include/linux/mtd/mtd.h \
    $(wildcard include/config/mtd/partitions.h) \
    $(wildcard include/config/mtd/debug.h) \
    $(wildcard include/config/mtd/debug/verbose.h) \
  include/linux/mtd/compatmac.h \
  include/mtd/mtd-abi.h \
  include/linux/blkdev.h \
    $(wildcard include/config/blk/dev/bsg.h) \
    $(wildcard include/config/bounce.h) \
  include/linux/major.h \
  include/linux/genhd.h \
    $(wildcard include/config/fail/make/request.h) \
    $(wildcard include/config/solaris/x86/partition.h) \
    $(wildcard include/config/bsd/disklabel.h) \
    $(wildcard include/config/unixware/disklabel.h) \
    $(wildcard include/config/minix/subpartition.h) \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
  include/asm/device.h \
  include/asm-generic/device.h \
  include/linux/pagemap.h \
  include/linux/mm.h \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/debug/pagealloc.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/security.h \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
  include/linux/binfmts.h \
  include/linux/shm.h \
  arch/ppc/include/asm/shmparam.h \
  arch/ppc/include/asm/shmbuf.h \
  include/linux/msg.h \
  arch/ppc/include/asm/msgbuf.h \
  include/linux/key.h \
  include/linux/xfrm.h \
  include/net/flow.h \
  include/linux/in6.h \
  include/asm/pgtable.h \
    $(wildcard include/config/fsl/booke.h) \
    $(wildcard include/config/ppc/std/mmu.h) \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/xmon.h) \
  include/asm-generic/4level-fixup.h \
  include/asm/io.h \
    $(wildcard include/config/ra.h) \
    $(wildcard include/config/rd.h) \
    $(wildcard include/config/8260/pci9.h) \
  include/asm/ibm4xx.h \
    $(wildcard include/config/acadia.h) \
    $(wildcard include/config/bubinga.h) \
    $(wildcard include/config/cpci405.h) \
    $(wildcard include/config/ep405.h) \
    $(wildcard include/config/kilauea.h) \
    $(wildcard include/config/makalu.h) \
    $(wildcard include/config/ppchameleonevb.h) \
    $(wildcard include/config/redwood/5.h) \
    $(wildcard include/config/redwood/6.h) \
    $(wildcard include/config/sc3.h) \
    $(wildcard include/config/sycamore.h) \
    $(wildcard include/config/taihu.h) \
    $(wildcard include/config/walnut.h) \
    $(wildcard include/config/xilinx/virtex.h) \
    $(wildcard include/config/alpr.h) \
    $(wildcard include/config/bamboo.h) \
    $(wildcard include/config/ebony.h) \
    $(wildcard include/config/katmai.h) \
    $(wildcard include/config/luan.h) \
    $(wildcard include/config/lwmon5.h) \
    $(wildcard include/config/ocotea.h) \
    $(wildcard include/config/p3p440.h) \
    $(wildcard include/config/sequoia.h) \
    $(wildcard include/config/rainier.h) \
    $(wildcard include/config/taishan.h) \
    $(wildcard include/config/yellowstone.h) \
    $(wildcard include/config/yosemite.h) \
    $(wildcard include/config/flyer.h) \
    $(wildcard include/config/yucca.h) \
  arch/ppc/include/asm/dcr.h \
    $(wildcard include/config/ppc/dcr.h) \
    $(wildcard include/config/ppc/dcr/native.h) \
    $(wildcard include/config/ppc/merge.h) \
  arch/ppc/include/asm/dcr-native.h \
  arch/ppc/platforms/4xx/flyer.h \
  arch/ppc/platforms/4xx/ibm440ep.h \
  include/asm/ibm44x.h \
    $(wildcard include/config/440sp.h) \
    $(wildcard include/config/440spe.h) \
    $(wildcard include/config/440ep.h) \
    $(wildcard include/config/440gr.h) \
    $(wildcard include/config/440epx.h) \
    $(wildcard include/config/440grx.h) \
    $(wildcard include/config/addr.h) \
    $(wildcard include/config/data.h) \
    $(wildcard include/config/440gp.h) \
    $(wildcard include/config/bank/enable.h) \
    $(wildcard include/config/size/mask.h) \
    $(wildcard include/config/bank/size.h) \
    $(wildcard include/config/size/8m.h) \
    $(wildcard include/config/size/16m.h) \
    $(wildcard include/config/size/32m.h) \
    $(wildcard include/config/size/64m.h) \
    $(wildcard include/config/size/128m.h) \
    $(wildcard include/config/size/256m.h) \
    $(wildcard include/config/size/512m.h) \
    $(wildcard include/config/size/1g.h) \
    $(wildcard include/config/size/2g.h) \
    $(wildcard include/config/size/4g.h) \
    $(wildcard include/config/440gx.h) \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/swap.h) \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/highmem.h \
  include/linux/uaccess.h \
  arch/ppc/include/asm/uaccess.h \
  arch/ppc/include/asm/cacheflush.h \
  arch/ppc/include/asm/kmap_types.h \
  include/linux/backing-dev.h \
  include/linux/mempool.h \
  include/linux/bio.h \
  include/linux/ioprio.h \
  include/linux/bsg.h \
  arch/ppc/include/asm/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  arch/ppc/include/asm/dma.h \
    $(wildcard include/config/ppc/iseries.h) \
    $(wildcard include/config/sound/cs4232.h) \
  include/linux/elevator.h \
  include/linux/blkpg.h \
  include/linux/freezer.h \
  include/linux/hdreg.h \
  include/linux/ata.h \
  include/linux/kthread.h \
  drivers/mtd/mtdcore.h \

drivers/mtd/mtd_blkdevs.o: $(deps_drivers/mtd/mtd_blkdevs.o)

$(deps_drivers/mtd/mtd_blkdevs.o):
