#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, World!\n");
        printf("\tWelcome to the kernel!\n");
}
