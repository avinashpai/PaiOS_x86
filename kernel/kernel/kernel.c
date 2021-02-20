#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/defs.h>

extern const char _kernel_start[], _kernel_end[];

void print_info(void) {
    printf("[");
    terminal_writestring_color("INFO", INFO);
    printf("] ");
}

void kernel_main(void) {
    gdt_install();
	terminal_initialize();
	printf("Initializing terminal...\n\n");
	printf("\tWelcome to PaiOS!\n");
	printf("        _,..---..,_\n"
           "    ,-\"`    .'.    `\"-,\n"
           "   ((      '.'.'      ))\n"
           "    `'-.,_   '   _,.-'`\n"
           "      `\\  `\"\"\"\"\"`  /`\n"
           "        `\"\"-----\"\"`\n\n");
	print_info();
    printf("Kernel loaded at [%x, %x]\n", _kernel_start, _kernel_end);
}
