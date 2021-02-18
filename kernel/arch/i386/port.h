//
// Created by Avinash Pai on 2/18/21.
//

#ifndef _PORT_H
#define _PORT_H

#include <stddef.h>


static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif //_PORT_H
