osname := $(shell uname -s)

CROSS_PREFIX=aarch64-linux-gnu-

all: kernel.elf

kernel.o: kernel.c
	$(CROSS_PREFIX)gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c kernel.c -o kernel.o

boot.o: boot.S
	$(CROSS_PREFIX)gcc -g -MMD -c boot.S -o boot.o

box.o: box.S
	$(CROSS_PREFIX)gcc -g -MMD -c box.S -o box.o

process.o: process.c
	$(CROSS_PREFIX)gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c process.c -o process.o

queue.o: queue.c
	$(CROSS_PREFIX)gcc -g -pedantic -Wall -Wextra -fPIC -std=gnu2x -MMD -c queue.c -o queue.o

process_asm.o: process_asm.S
	$(CROSS_PREFIX)gcc -g -MMD -c process_asm.S -o process_asm.o

kernel.elf: kernel.o boot.o box.o process.o queue.o process_asm.o libos.a  
	$(CROSS_PREFIX)ld -g -N -Ttext=0x10000 -o kernel.elf kernel.o boot.o box.o process.o queue.o process_asm.o libos.a

run:
	qemu-system-aarch64 -machine raspi3b   -kernel kernel.elf
	
debug:
	qemu-system-aarch64 -machine raspi3b  -S -s -kernel kernel.elf &
	ddd --debugger 'gdb-multiarch -ex "target remote localhost:1234" -ex "break main" -ex "continue"' kernel.elf
	
clean:
	rm -f kernel.elf kernel.o boot.o box.o process.o queue.o process_asm.o kernel.d boot.d box.d process.d queue.d process_asm.d
