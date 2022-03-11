cmd_kernel/time/jiffies.o := ppc_4xxFP-gcc -m32 -Wp,-MD,kernel/time/.jiffies.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(jiffies)"  -D"KBUILD_MODNAME=KBUILD_STR(jiffies)" -c -o kernel/time/jiffies.o kernel/time/jiffies.c

deps_kernel/time/jiffies.o := \
  kernel/time/jiffies.c \
  include/linux/clocksource.h \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/clocksource/watchdog.h) \
    $(wildcard include/config/generic/time/vsyscall.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/lsf.h) \
    $(wildcard include/config/resources/64bit.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/forced/inlining.h) \
  include/linux/compiler-gcc.h \
  arch/ppc/include/asm/posix_types.h \
  arch/ppc/include/asm/types.h \
  include/linux/timex.h \
    $(wildcard include/config/no/hz.h) \
  include/linux/time.h \
  include/linux/cache.h \
    $(wildcard include/config/smp.h) \
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
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  arch/ppc/include/asm/synch.h \
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
  arch/ppc/include/asm/cache.h \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/403gcx.h) \
    $(wildcard include/config/ppc32.h) \
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
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
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
  arch/ppc/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  arch/ppc/include/asm/timex.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/calc64.h \
  arch/ppc/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/asm/io.h \
    $(wildcard include/config/ra.h) \
    $(wildcard include/config/rd.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/8260/pci9.h) \
  include/linux/string.h \
  arch/ppc/include/asm/string.h \
  include/asm/mmu.h \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/serial/text/debug.h) \
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
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \

kernel/time/jiffies.o: $(deps_kernel/time/jiffies.o)

$(deps_kernel/time/jiffies.o):
