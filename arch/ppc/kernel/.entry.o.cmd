cmd_arch/ppc/kernel/entry.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/ppc/kernel/.entry.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2     -c -o arch/ppc/kernel/entry.o arch/ppc/kernel/entry.S

deps_arch/ppc/kernel/entry.o := \
  arch/ppc/kernel/entry.S \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/6xx.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/44x.h) \
    $(wildcard include/config/ibm405/err77.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/errno.h \
  arch/ppc/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/sys.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  arch/ppc/include/asm/processor.h \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/ppc64.h) \
  arch/ppc/include/asm/reg.h \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/apus/fast/except.h) \
    $(wildcard include/config/ppc/cell.h) \
  include/linux/stringify.h \
  arch/ppc/include/asm/cputable.h \
    $(wildcard include/config/mpc10x/bridge.h) \
    $(wildcard include/config/ppc/83xx.h) \
    $(wildcard include/config/8260.h) \
    $(wildcard include/config/bdi/switch.h) \
    $(wildcard include/config/power3.h) \
    $(wildcard include/config/power4.h) \
    $(wildcard include/config/ppc/pasemi/a2/workarounds.h) \
    $(wildcard include/config/e200.h) \
    $(wildcard include/config/e500.h) \
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/power4/only.h) \
  arch/ppc/include/asm/types.h \
  include/asm/reg_booke.h \
    $(wildcard include/config/440a.h) \
    $(wildcard include/config/403gcx.h) \
  include/asm/page.h \
    $(wildcard include/config/ppc/page/4k.h) \
    $(wildcard include/config/ppc/page/16k.h) \
    $(wildcard include/config/ppc/page/64k.h) \
    $(wildcard include/config/ppc/page/256k.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/pte/64bit.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
  include/asm/mmu.h \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/serial/text/debug.h) \
  arch/ppc/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  arch/ppc/include/asm/ppc_asm.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/ppc601/sync/fix.h) \
    $(wildcard include/config/ibm440ep/err42.h) \
  include/asm/asm-offsets.h \
  arch/ppc/include/asm/unistd.h \
  arch/ppc/kernel/head_booke.h \

arch/ppc/kernel/entry.o: $(deps_arch/ppc/kernel/entry.o)

$(deps_arch/ppc/kernel/entry.o):
