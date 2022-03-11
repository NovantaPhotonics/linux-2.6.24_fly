cmd_arch/powerpc/kernel/vdso32/vdso32.lds := ppc_4xxFP-gcc -m32 -E -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign -Wp,-MD,arch/powerpc/kernel/vdso32/.vdso32.lds.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include   -P -C -Upowerpc -D__ASSEMBLY__ -o arch/powerpc/kernel/vdso32/vdso32.lds arch/powerpc/kernel/vdso32/vdso32.lds.S

deps_arch/powerpc/kernel/vdso32/vdso32.lds := \
  arch/powerpc/kernel/vdso32/vdso32.lds.S \
    $(wildcard include/config/ppc64.h) \
  arch/ppc/include/asm/vdso.h \

arch/powerpc/kernel/vdso32/vdso32.lds: $(deps_arch/powerpc/kernel/vdso32/vdso32.lds)

$(deps_arch/powerpc/kernel/vdso32/vdso32.lds):
