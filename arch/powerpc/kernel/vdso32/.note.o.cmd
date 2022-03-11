cmd_arch/powerpc/kernel/vdso32/note.o := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/powerpc/kernel/vdso32/.note.o.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -D__ASSEMBLY__ -Iarch/ppc -Wa,-m405 -gdwarf-2  -D__VDSO32__ -s   -c -o arch/powerpc/kernel/vdso32/note.o arch/powerpc/kernel/vdso32/note.S

deps_arch/powerpc/kernel/vdso32/note.o := \
  arch/powerpc/kernel/vdso32/note.S \
  include/linux/uts.h \
  include/linux/version.h \

arch/powerpc/kernel/vdso32/note.o: $(deps_arch/powerpc/kernel/vdso32/note.o)

$(deps_arch/powerpc/kernel/vdso32/note.o):
