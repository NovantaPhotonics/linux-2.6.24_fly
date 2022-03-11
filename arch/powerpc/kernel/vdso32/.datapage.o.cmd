cmd_arch/powerpc/kernel/vdso32/datapage.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/powerpc/kernel/vdso32/.datapage.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2  -D__VDSO32__ -s   -c -o arch/powerpc/kernel/vdso32/datapage.o arch/powerpc/kernel/vdso32/datapage.S

deps_arch/powerpc/kernel/vdso32/datapage.o := \
  arch/powerpc/kernel/vdso32/datapage.S \
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
  arch/ppc/include/asm/types.h \
  include/asm/reg_booke.h \
    $(wildcard include/config/440a.h) \
    $(wildcard include/config/403gcx.h) \
  arch/ppc/include/asm/ppc_asm.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/ppc601/sync/fix.h) \
    $(wildcard include/config/ibm440ep/err42.h) \
  include/asm/asm-offsets.h \
  arch/ppc/include/asm/unistd.h \
  arch/ppc/include/asm/vdso.h \

arch/powerpc/kernel/vdso32/datapage.o: $(deps_arch/powerpc/kernel/vdso32/datapage.o)

$(deps_arch/powerpc/kernel/vdso32/datapage.o):
