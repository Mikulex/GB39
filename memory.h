#ifndef MEMORY_HEADER_INCLUDED
#define MEMORY_HEADER_INCLUDED

#include <stdint.h>

// Memory Map
#define BANK_ZERO_START 0x0000 // 16 KiB ROM bank 00
#define BANK_ZERO_END 0x3FFF
#define BANK_N_START 0x4000 // 16 KiB ROM Bank 01~NN
#define BANK_N_END 0x7FFF
#define VRAM_START 0x8000 // 8 KiB Video RAM (VRAM)
#define VRAM_END 0x9FFF
#define EXTERNAL_RAM_START 0xA000 // 8 KiB External RAM
#define EXTERNAL_RAM_END 0xBFFF
#define WRAM_ZERO_START 0xC000 // 4 KiB Work RAM (WRAM)
#define WRAM_ZERO_END 0xCFFF
#define WRAM_N_START 0xD000 // 4 KiB Work RAM (WRAM)
#define WRAM_N_END 0xDFFF
#define MIRROR_Z_START 0xE000 // Mirror of C000~CFFF (ECHO RAM)
#define MIRROR_Z_END 0xEFFF
#define MIRROR_N_START 0xF000 // Mirror of D000~DDFF (ECHO RAM)
#define MIRROR_N_END 0xFDFF
#define OAM_START 0xFE00 // Object attribute memory (OAM)
#define OAM_END 0xFE9F
#define UNUSED_START 0xFEA0 // Not Usable
#define UNUSED_END 0xFEFF
#define IO_START 0xFF00 // I/O Registers
#define IO_END 0xFF7F
#define HRAM_START 0xFF80 // High RAM (HRAM)
#define HRAM_END 0xFFFE
#define IE_START 0xFFFF // Interrupt Enable register (IE)
#define IE_END 0xFFFF

typedef struct hardware_regs {
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
  uint8_t LCDC;
  uint8_t LY;
  uint8_t LYC;
  uint8_t STAT;
  uint8_t SCY;
  uint8_t SCX;
  uint8_t WY;
  uint8_t WX;
  uint8_t BGP;
  uint8_t VBK;
  uint8_t disable_boot_rom;
  uint8_t HDMA1;
  uint8_t HDMA2;
  uint8_t HDMA3;
  uint8_t HDMA4;
  uint8_t HDMA5;
  uint8_t BCPS_BGPI;
  uint8_t BCPD_BGPD;
  uint8_t OCPS_OBPI;
  uint8_t OCPD_OBPD;
  uint8_t CVBK;
  uint8_t IME;
} hardware_regs;

typedef union hardware {
  hardware_regs regs;
  uint8_t mappings[sizeof(hardware_regs) / sizeof(uint8_t)];
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
