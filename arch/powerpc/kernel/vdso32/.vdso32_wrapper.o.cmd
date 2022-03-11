cmd_arch/powerpc/kernel/vdso32/vdso32_wrapper.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/powerpc/kernel/vdso32/.vdso32_wrapper.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2  -D__VDSO32__ -s   -c -o arch/powerpc/kernel/vdso32/vdso32_wrapper.o arch/powerpc/kernel/vdso32/vdso32_wrapper.S

deps_arch/powerpc/kernel/vdso32/vdso32_wrapper.o := \
  arch/powerpc/kernel/vdso32/vdso32_wrapper.S \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
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

arch/powerpc/kernel/vdso32/vdso32_wrapper.o: $(deps_arch/powerpc/kernel/vdso32/vdso32_wrapper.o)

$(deps_arch/powerpc/kernel/vdso32/vdso32_wrapper.o):
