/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
    /* The kernel will be at 3GB + 1MB in the virtual address space,
        mapping to 1MB in the physical address space. */
	. = 0xC0100000;

    _kernel_start = .;
    .text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
    {
       *(.multiboot.data)
       *(.multiboot.text)
       *(.text)
    }
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
    {
        *(.rodata)
    }
    .data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
    {
        *(.data)
    }
    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    _kernel_end = .;
}
