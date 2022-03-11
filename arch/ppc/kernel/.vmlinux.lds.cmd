cmd_arch/ppc/kernel/vmlinux.lds := ppc_4xxFP-gcc -m32 -E -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign -Wp,-MD,arch/ppc/kernel/.vmlinux.lds.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include   -Upowerpc -P -C -Uppc -D__ASSEMBLY__ -o arch/ppc/kernel/vmlinux.lds arch/ppc/kernel/vmlinux.lds.S

deps_arch/ppc/kernel/vmlinux.lds := \
  arch/ppc/kernel/vmlinux.lds.S \
    $(wildcard include/config/blk/dev/initrd.h) \
  include/asm-generic/vmlinux.lds.h \
  include/asm/page.h \
    $(wildcard include/config/ppc/page/4k.h) \
    $(wildcard include/config/ppc/page/16k.h) \
    $(wildcard include/config/ppc/page/64k.h) \
    $(wildcard include/config/ppc/page/256k.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/pte/64bit.h) \
  arch/ppc/include/asm/asm-compat.h \
    $(wildcard include/config/ppc64.h) \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  arch/ppc/include/asm/types.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \

arch/ppc/kernel/vmlinux.lds: $(deps_arch/ppc/kernel/vmlinux.lds)

$(deps_arch/ppc/kernel/vmlinux.lds):
