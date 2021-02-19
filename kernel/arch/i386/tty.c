#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include "port.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xC03FF000;

static const uint16_t REG_SCREEN_CTRL = 0x3D4;
static const uint16_t REG_SCREEN_DATA = 0x3D5;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

static size_t offset(size_t x, size_t y) { return y * VGA_WIDTH + x; }

void enable_cursor(uint8_t start, uint8_t end) {
    outb(REG_SCREEN_CTRL, 0x0A);
    outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xC0) | start);

    outb(REG_SCREEN_CTRL, 0x0B);
    outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xE0) | end);
}

void update_cursor(size_t x, size_t y) {
    uint16_t pos = offset(x, y);

    outb(REG_SCREEN_CTRL, 0x0F);
    outb(REG_SCREEN_DATA, (uint8_t)(pos & 0xFF));
    outb(REG_SCREEN_CTRL, 0x0E);
    outb(REG_SCREEN_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_DARK_GREY);
  terminal_buffer = VGA_MEMORY;
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
      const size_t index = offset(x, y);
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }

  enable_cursor(0, 15);
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
  const size_t index = offset(x, y);
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear(void) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        for (size_t y = 0; y < VGA_HEIGHT; y++) {
            terminal_putentryat(' ', terminal_color, x, y);
        }
    }

    update_cursor(0, 0);
}

void terminal_putchar(char c) {
  unsigned char uc = c;
  switch (uc) {
    case '\n': {  // newline
        terminal_column = 0;
        terminal_row++;
        break;
    }
    case '\t': {  // tabs
        terminal_column += 4;
        break;
    }
    default: {
        terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
        terminal_column++;
        break;
    }
  }

  // Scrolling
  if (terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    terminal_row++;
  }

  if (terminal_row == VGA_HEIGHT) {
    for (size_t i = 1; i < VGA_HEIGHT; i++) {
      memcpy(terminal_buffer + offset(0, i - 1),
             terminal_buffer + offset(0, i), VGA_WIDTH * 2);
    }
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT-1);
    }

    terminal_row--;
  }

  update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

