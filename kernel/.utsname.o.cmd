cmd_kernel/utsname.o := ppc_4xxFP-gcc -m32 -Wp,-MD,kernel/.utsname.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(utsname)"  -D"KBUILD_MODNAME=KBUILD_STR(utsname)" -c -o kernel/utsname.o kernel/utsname.c

deps_kernel/utsname.o := \
  kernel/utsname.c \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/forced/inlining.h) \
  include/linux/compiler-gcc.h \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/lsf.h) \
    $(wildcard include/config/resources/64bit.h) \
  include/linux/posix_types.h \
  arch/ppc/include/asm/posix_types.h \
  arch/ppc/include/asm/types.h \
  arch/ppc/include/asm/processor.h \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
  arch/ppc/include/asm/reg.h \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/apus/fast/except.h) \
    $(wildcard include/config/ppc/cell.h) \
  include/linux/stringify.h \
  arch/ppc/include/asm/cputable.h \
    $(wildcard include/config/smp.h) \
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
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  include/asm/reg_booke.h \
    $(wildcard include/config/440a.h) \
    $(wildcard include/config/403gcx.h) \
  arch/ppc/include/asm/ptrace.h \
  arch/ppc/include/asm/cache.h \
  include/asm/system.h \
    $(wildcard include/config/6xx.h) \
    $(wildcard include/config/booke/wdt.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/numa.h) \
  /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include/stdarg.h \
  include/linux/linkage.h \
  arch/ppc/include/asm/linkage.h \
  include/linux/bitops.h \
  arch/ppc/include/asm/bitops.h \
  arch/ppc/include/asm/synch.h \
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
  arch/ppc/include/asm/hw_irq.h \
  include/linux/errno.h \
  arch/ppc/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/stat.h \
  arch/ppc/include/asm/stat.h \
  include/linux/time.h \
  include/linux/cache.h \
  include/linux/seqlock.h \
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
  include/asm/page.h \
    $(wildcard include/config/ppc/page/4k.h) \
    $(wildcard include/config/ppc/page/16k.h) \
    $(wildcard include/config/ppc/page/64k.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/pte/64bit.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
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
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/hotplug/cpu.h) \
  arch/ppc/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/capability.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/timex.h \
  arch/ppc/include/asm/timex.h \
  include/linux/jiffies.h \
  include/linux/calc64.h \
  arch/ppc/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/rbtree.h \
  include/linux/cpumask.h \
  include/linux/bitmap.h \
  include/linux/string.h \
  arch/ppc/include/asm/string.h \
  include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/mmu.h) \
  include/linux/auxvec.h \
  arch/ppc/include/asm/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  arch/ppc/include/asm/rwsem.h \
  include/linux/completion.h \
  include/linux/wait.h \
  arch/ppc/include/asm/current.h \
  include/asm/mmu.h \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/serial/text/debug.h) \
  arch/ppc/include/asm/semaphore.h \
  arch/ppc/include/asm/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime.h \
  include/linux/smp.h \
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
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slabinfo.h) \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  arch/ppc/include/asm/topology.h \
    $(wildcard include/config/pci.h) \
  include/asm-generic/topology.h \
  include/linux/slab_def.h \
  include/linux/kmalloc_sizes.h \
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
  include/linux/uts.h \
  include/linux/utsname.h \
  include/linux/nsproxy.h \
    $(wildcard include/config/cgroup/ns.h) \
  include/linux/version.h \
  include/linux/err.h \

kernel/utsname.o: $(deps_kernel/utsname.o)

$(deps_kernel/utsname.o):
