cmd_drivers/serial/built-in.o :=  ppc_4xxFP-ld -m elf32ppc    -r -o drivers/serial/built-in.o drivers/serial/serial_core.o drivers/serial/8250.o drivers/serial/8250_pci.o drivers/serial/8250_early.o
