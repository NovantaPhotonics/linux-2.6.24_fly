cmd_arch/ppc/kernel/misc.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/ppc/kernel/.misc.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2     -c -o arch/ppc/kernel/misc.o arch/ppc/kernel/misc.S

deps_arch/ppc/kernel/misc.o := \
  arch/ppc/kernel/misc.S \
    $(wildcard include/config/44x.h) \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/fsl/booke.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/403gcx.h) \
    $(wildcard include/config/not/coherent/cache.h) \
  include/linux/sys.h \
  arch/ppc/include/asm/unistd.h \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc64.h) \
  arch/ppc/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  arch/ppc/include/asm/processor.h \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
  arch/ppc/include/asm/reg.h \
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
    $(wildcard include/config/ibm405/err77.h) \
  arch/ppc/include/asm/types.h \
  include/asm/reg_booke.h \
    $(wildcard include/config/440a.h) \
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
  include/asm/ppc_page_asm.h \
  arch/ppc/include/asm/cache.h \
  include/asm/mmu.h \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/serial/text/debug.h) \
  arch/ppc/include/asm/ppc_asm.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/ppc601/sync/fix.h) \
    $(wildcard include/config/ibm440ep/err42.h) \
  arch/ppc/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/asm-offsets.h \
  include/asm/ibm44x.h \
    $(wildcard include/config/ppc/merge.h) \
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
  arch/ppc/include/asm/dcr-native.h \
  arch/ppc/platforms/4xx/flyer.h \
  arch/ppc/platforms/4xx/ibm440ep.h \

arch/ppc/kernel/misc.o: $(deps_arch/ppc/kernel/misc.o)

$(deps_arch/ppc/kernel/misc.o):
