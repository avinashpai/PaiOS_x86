#include <stdio.h>

#include <kernel/tty.h>

extern const char _kernel_start[], _kernel_end[];

void kernel_main(void) {
	terminal_initialize();
	printf("Initializing terminal...\n\n");
	printf("\tWelcome to PaiOS!\n");
	printf("        _,..---..,_\n"
           "    ,-\"`    .'.    `\"-,\n"
           "   ((      '.'.'      ))\n"
           "    `'-.,_   '   _,.-'`\n"
           "      `\\  `\"\"\"\"\"`  /`\n"
           "        `\"\"-----\"\"`\n\n");
	printf("[");
	terminal_writestring_color("INFO", INFO);
	printf("] ");
    printf("Kernel loaded at [%x, %x]\n", _kernel_start, _kernel_end);
}
