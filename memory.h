#ifndef MEMORY_HEADER_INCLUDED
#define MEMORY_HEADER_INCLUDED

#include <stdint.h>

extern uint16_t boot_rom[256];
extern uint16_t vram[8 * 1024];

typedef struct memory_map {

} memory_map;

typedef struct cartridge {
  uint8_t *rom;
  uint8_t *ram;
  size_t rom_c;
  size_t ram_c;
} cartridge;

#endif
