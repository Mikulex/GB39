#ifndef MEMORY_HEADER_INCLUDED
#define MEMORY_HEADER_INCLUDED

#include <stdint.h>

// Memory Map
#define BANK_ZERO_ADDR 0x0000    // 16 KiB ROM bank 00
#define BANK_N_ADDR 0x4000       // 16 KiB ROM Bank 01~NN
#define VRAM_ADDR 0x8000         // 8 KiB Video RAM (VRAM)
#define EXTERNAL_RAM_ADDR 0xA000 // 8 KiB External RAM
#define WRAM_ZERO_ADDR 0xC000    // 4 KiB Work RAM (WRAM)
#define WRAM_N_ADDR 0xD000       // 4 KiB Work RAM (WRAM)
#define MIRROR_ADDR 0xE000       // Mirror of C000~DDFF (ECHO RAM)
#define OAM_ADDR 0xFE00          // Object attribute memory (OAM)
#define UNUSED_ADDR 0xFEA0       // Not Usable
#define IO_ADDR 0xFF00           // I/O Registers
#define HRAM_ADDR 0xFF80         // High RAM (HRAM)
#define IE_ADDR 0xFFFF           // Interrupt Enable register (IE)

typedef struct hardware {
  uint8_t JOYP;
  uint8_t SB;
  uint8_t SC;
  uint8_t DIV;
  uint8_t TIMA;
  uint8_t TMA;
  uint8_t TAC;
  uint8_t NR52;
  uint8_t NR51;
  uint8_t NR50;
  uint8_t NR10;
  uint8_t NR11;
  uint8_t NR12;
  uint8_t NR13;
  uint8_t NR14;
  uint8_t NR21;
  uint8_t NR22;
  uint8_t NR23;
  uint8_t NR24;
  uint8_t NR31;
  uint8_t NR32;
  uint8_t NR33;
  uint8_t NR34;
  uint8_t wav_ram[2];
  uint8_t NR42;
  uint8_t NR43;
  uint8_t NR44;
} hardware;

typedef struct bank {
  uint8_t *mem;
  size_t mem_c;
} bank;

typedef struct bus {
  bank bank_z;
  bank bank_n;
  bank vram;
  bank ext_ram;
  bank wram_z;
  bank wram_n;
  bank oam;
  bank hram;
  hardware hdw;
} bus;

typedef struct cartridge {
  uint8_t *rom;
  uint8_t *ram;
  size_t rom_c;
  size_t ram_c;
} cartridge;

#endif
