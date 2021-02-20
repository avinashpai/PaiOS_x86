# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot.data
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Preallocate pages
.section .bss, "aw", @nobits
    .align 4096
boot_page_directory:
    .skip 4096
boot_page_table1:
    .skip 4096

# The kernel entry point.
.section .multiboot.text
.global _start
.type _start, @function
_start:
    # Physical address of boot_page_table1.
    # TODO: I recall seeing some assembly that used a macro to do the
    #       conversions to and from physical. Maybe this should be done in this
    #       code as well?
    movl $(boot_page_table1 - 0xC0000000), %edi
    # TODO: Start at the first kernel page instead. Alternatively map the first
    #       1 MiB as it can be generally useful, and there's no need to
    #       specially map the VGA buffer.

1:
    # map kernel
    cmpl $(_kernel_start - 0xC0000000), %esi
    jl 2f
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f

    # mapy phy address as "present, writable".
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

2:
    # 4k page
    addl $4096, %esi
    # 4 byte entry
    addl $4, %edi
    loop 1b

3:
    # Map VGA memory
    movl $(0x000B8000 | 0x003 ), boot_page_table1 - 0xC0000000 + 1023 * 4

    # Identity mapping
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

    # Set CR3
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

    # Enable paging and the write-protect bit
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

.section .text

4:
    # Paging is enabled
    # Unmap identity mapping
    movl $0, boot_page_directory + 0

    # Reload CR3 to force TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Set up stack
	movl $stack_top, %esp

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b

.global gdt_flush
.extern gp
gdt_flush:
    lgdt (gp)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    ljmp $0x08, $flush2
flush2:
    ret