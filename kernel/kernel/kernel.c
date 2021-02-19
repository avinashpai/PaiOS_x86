#include <stdio.h>

#include <kernel/tty.h>

extern const char _kernel_start[], _kernel_end[];

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, World!\n");
	printf("\tWelcome to the kernel!\n");
	printf("\tKernel loaded at [%x, %x]\n", _kernel_start, _kernel_end);
}
