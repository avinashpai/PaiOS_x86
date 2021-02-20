#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

typedef enum{
    INFO,
    SUCCESS,
    WARNING,
    FAILURE
} msg_type;

void terminal_initialize(void);
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_writestring_color(const char* data, msg_type t);


#endif
