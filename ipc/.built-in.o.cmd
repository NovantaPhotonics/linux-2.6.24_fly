cmd_ipc/built-in.o :=  ppc_4xxFP-ld -m elf32ppc    -r -o ipc/built-in.o ipc/util.o ipc/msgutil.o ipc/msg.o ipc/sem.o ipc/shm.o ipc/ipc_sysctl.o
