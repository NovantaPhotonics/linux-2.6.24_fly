cmd_arch/powerpc/kernel/vdso32/vdso32.so.dbg := ppc_4xxFP-gcc -m32 -Wp,-MD,arch/powerpc/kernel/vdso32/.vdso32.so.dbg.d  -nostdinc -isystem /home/mh1/MarkingHeads/amcc/usr/bin/../lib/gcc/powerpc-linux/4.2.2/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Iarch/ppc -Iarch/ppc/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -O2 -Iarch/ppc -msoft-float -pipe -ffixed-r2 -mmultiple  -mno-altivec -mstring -Wa,-m405 -fomit-frame-pointer -g  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign  -shared -fno-common -fno-builtin -nostdlib -Wl,-soname=linux-vdso32.so.1  -Wl,--hash-style=sysv   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(vdso32.so)"  -D"KBUILD_MODNAME=KBUILD_STR(vdso32.so)" -Wl,-T arch/powerpc/kernel/vdso32/vdso32.lds arch/powerpc/kernel/vdso32/sigtramp.o arch/powerpc/kernel/vdso32/gettimeofday.o arch/powerpc/kernel/vdso32/datapage.o arch/powerpc/kernel/vdso32/cacheflush.o arch/powerpc/kernel/vdso32/note.o -o arch/powerpc/kernel/vdso32/vdso32.so.dbg
