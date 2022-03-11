cmd_arch/powerpc/kernel/systbl.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/powerpc/kernel/.systbl.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2     -c -o arch/powerpc/kernel/systbl.o arch/powerpc/kernel/systbl.S

deps_arch/powerpc/kernel/systbl.o := \
  arch/powerpc/kernel/systbl.S \
    $(wildcard include/config/ppc64.h) \
  arch/ppc/include/asm/ppc_asm.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/ppc601/sync/fix.h) \
    $(wildcard include/config/ppc/cell.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/ibm440ep/err42.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/40x.h) \
  include/linux/stringify.h \
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  arch/ppc/include/asm/types.h \
  arch/ppc/include/asm/systbl.h \

arch/powerpc/kernel/systbl.o: $(deps_arch/powerpc/kernel/systbl.o)

$(deps_arch/powerpc/kernel/systbl.o):
