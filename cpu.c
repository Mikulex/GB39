#include "cpu.h"
#include <stdbool.h>
#include <stdint.h>

#define FLAG_MASK_Z 1u
#define FLAG_MASK_N (1u << 1)
#define FLAG_MASK_H (1u << 2)
#define FLAG_MASK_C (1u << 3)
#define BIT_MASK_3 (1u << 3)
#define BIT_MASK_7 (1u << 7)
#define BIT_MASK_11 (1u << 11)
#define BIT_MASK_15 (1u << 15)

// clang-format off
uint8_t CYCLE_LENGHTS[] = {
//  0   1   2   3   4   5   6   7   8   9   a   b  c   d   e  f
    4,  12, 8,  8,  4,  4,  8,  4,  20, 8,  8,  8, 4,  4,  8, 4,  // 0
    4,  12, 8,  8,  4,  4,  8,  4,  12, 8,  8,  8, 4,  4,  8, 4,  // 1
    8,  12, 8,  8,  4,  4,  8,  4,  8,  8,  8,  8, 4,  4,  8, 4,  // 2
    8,  12, 8,  8,  12, 12, 12, 4,  8,  8,  8,  8, 4,  4,  8, 4,  // 3
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // 4
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // 5
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // 6
    8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4, 4,  4,  8, 4,  // 7
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // 8
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // 9
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // a
    4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4, 4,  4,  8, 4,  // b
    8,  12, 12, 16, 12, 16, 8,  16, 8,  16, 12, 4, 12, 24, 8, 16, // c
    8,  12, 12, 0,  12, 16, 8,  16, 8,  16, 12, 0, 12, 0,  8, 16, // d
    12, 12, 8,  0,  0,  16, 8,  16, 16, 4,  16, 0, 0,  0,  8, 16, // e
    12, 12, 8,  4,  0,  16, 8,  16, 12, 8,  16, 4, 0,  0,  8, 16  // f
};
// clang-format off

void start(uint8_t **boot_rom, uint8_t **ram, uint8_t **vram) {
  registers regs;
  context ctx;

  bool running = true;
  while (running) {
    uint8_t op = fetch_imm_n(&regs.PC, ram);
    ctx.cycle += CYCLE_LENGHTS[op];

    switch (op) {
    case 0x00:
      // noop
      break;
    case 0x01:
      ld_rr_nn(&regs.BC, fetch_imm_nn(&regs.PC, ram));
      break;
    case 0x02:
      ld_indr_r(regs.BC, &regs.A, ram);
      break;
    case 0x03:
      inc_rr(&regs.BC);
      break;
    case 0x04:
      inc_r(&regs.B, &regs.F);
      break;
    case 0x05:
      dec_r(&regs.B, &regs.F);
      break;
    case 0x06:
      ld_r_n(&regs.B, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x07:
      rlca(&regs.A, &regs.F);
      break;
    case 0x08:
      ld_indr_rr(fetch_imm_nn(&regs.PC, ram), &regs.SP, ram);
      break;
    case 0x09:
      add_rr_rr(&regs.HL, &regs.BC, &regs.F);
      break;
    case 0x0a:
      ld_r_indr(&regs.A, regs.BC, ram);
      break;
    case 0x0b:
      dec_rr(&regs.BC);
      break;
    case 0x0c:
      inc_r(&regs.C, &regs.F);
      break;
    case 0x0d:
      dec_r(&regs.C, &regs.F);
      break;
    case 0x0e:
      ld_r_n(&regs.C, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x0f:
      rrca(&regs.A, &regs.F);
      break;
    case 0x10:
      stop(0x00);
      break;
    case 0x11:
      ld_rr_nn(&regs.DE, fetch_imm_nn(&regs.PC, ram));
      break;
    case 0x12:
      inc_rr(&regs.DE);
      break;
    case 0x13:
      inc_r(&regs.D, &regs.F);
      break;
    case 0x14:
      dec_r(&regs.D, &regs.F);
      break;
    case 0x15:
      ld_r_n(&regs.D, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x16:
      ld_r_n(&regs.D, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x17:
      rla(&regs.A, &regs.F);
      break;
    case 0x18:
      jr(&regs.PC, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x19:
      add_rr_rr(&regs.HL, &regs.DE, &regs.F);
      break;
    case 0x1a:
      ld_r_indr(&regs.A, regs.DE, ram);
      break;
    case 0x1b:
      dec_rr(&regs.DE);
      break;
    case 0x1c:
      inc_r(&regs.E, &regs.F);
      break;
    case 0x1d:
      dec_r(&regs.E, &regs.F);
      break;
    case 0x1e:
      ld_r_n(&regs.E, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x1f:
      rra(&regs.A, &regs.F);
      break;
    case 0x20:
      jr_nz(&regs.F, &regs.PC, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x21:
      ld_rr_nn(&regs.HL, fetch_imm_nn(&regs.PC, ram));
      break;
    case 0x22:
      ld_indr_r(regs.HL, &regs.A, ram);
      regs.HL++;
      break;
    case 0x23:
      inc_rr(&regs.HL);
      break;
    case 0x24:
      inc_r(&regs.H, &regs.F);
      break;
    case 0x25:
      dec_r(&regs.H, &regs.F);
      break;
    case 0x26:
      ld_r_n(&regs.H, fetch_imm_n(&regs.PC, ram));
      regs.PC++;
      break;
    case 0x27:
      daa(&regs.A, &regs.F);
      break;
    case 0x28:
      jr_z(&regs.F, &regs.PC, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x29:
      ld_rr_rr(&regs.HL, &regs.HL);
      break;
    case 0x2a:
      ld_r_indr(&regs.A, regs.HL, ram);
      regs.HL++;
      break;
    case 0x2b:
      dec_rr(&regs.HL);
      break;
    case 0x2c:
      inc_r(&regs.L, &regs.F);
      break;
    case 0x2d:
      dec_r(&regs.L, &regs.F);
      break;
    case 0x2e:
      ld_r_n(&regs.L, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x2f:
      cpl(&regs.A, &regs.F);
      break;
    case 0x30:
      jr_nc(&regs.F, &regs.PC, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x31:
      ld_rr_nn(&regs.SP, fetch_imm_nn(&regs.PC, ram));
      break;
    case 0x32:
      ld_indr_r(regs.HL, &regs.A, ram);
      regs.HL--;
      break;
    case 0x33:
      inc_rr(&regs.SP);
      break;
    case 0x34:
      inc_indr(regs.HL, &regs.F, ram);
      break;
    case 0x35:
      dec_indr(regs.HL, &regs.F, ram);
      break;
    case 0x36:
      ld_indr_nn(regs.HL, fetch_imm_n(&regs.PC, ram), ram);
      break;
    case 0x37:
      scf(&regs.F);
      break;
    case 0x38:
      jr_c(&regs.F, &regs.PC, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x39:
      add_rr_rr(&regs.HL, &regs.SP, &regs.F);
      break;
    case 0x3a:
      ld_r_indr(&regs.A, regs.HL, ram);
      regs.HL--;
      break;
    case 0x3b:
      dec_rr(&regs.SP);
      break;
    case 0x3c:
      inc_r(&regs.A, &regs.F);
      break;
    case 0x3d:
      dec_r(&regs.A, &regs.F);
      break;
    case 0x3e:
      ld_r_n(&regs.A, fetch_imm_n(&regs.PC, ram));
      break;
    case 0x3f:
      ccf(&regs.F);
      break;
    case 0x40:
      ld_r_r(&regs.B, &regs.B);
      break;
    case 0x41:
      ld_r_r(&regs.B, &regs.C);
      break;
    case 0x42:
      ld_r_r(&regs.B, &regs.D);
      break;
    case 0x43:
      ld_r_r(&regs.B, &regs.E);
      break;
    case 0x44:
      ld_r_r(&regs.B, &regs.H);
      break;
    case 0x45:
      ld_r_r(&regs.B, &regs.L);
      break;
    case 0x46:
      ld_r_indr(&regs.B, regs.HL, ram);
      break;
    case 0x47:
      ld_r_r(&regs.B, &regs.A);
      break;
    case 0x48:
      ld_r_r(&regs.C, &regs.B);
      break;
    case 0x49:
      ld_r_r(&regs.C, &regs.C);
      break;
    case 0x4a:
      ld_r_r(&regs.C, &regs.D);
      break;
    case 0x4b:
      ld_r_r(&regs.C, &regs.E);
      break;
    case 0x4c:
      ld_r_r(&regs.C, &regs.H);
      break;
    case 0x4d:
      ld_r_r(&regs.C, &regs.L);
      break;
    case 0x4e:
      ld_r_indr(&regs.C, regs.HL, ram);
      break;
    case 0x4f:
      ld_r_r(&regs.C, &regs.A);
      break;
    case 0x50:
      ld_r_r(&regs.D, &regs.B);
      break;
    case 0x51:
      ld_r_r(&regs.D, &regs.C);
      break;
    case 0x52:
      ld_r_r(&regs.D, &regs.D);
      break;
    case 0x53:
      ld_r_r(&regs.D, &regs.E);
      break;
    case 0x54:
      ld_r_r(&regs.D, &regs.H);
      break;
    case 0x55:
      ld_r_r(&regs.D, &regs.L);
      break;
    case 0x56:
      ld_r_indr(&regs.D, regs.HL, ram);
      break;
    case 0x57:
      ld_r_r(&regs.D, &regs.A);
      break;
    case 0x58:
      ld_r_r(&regs.E, &regs.B);
      break;
    case 0x59:
      ld_r_r(&regs.E, &regs.C);
      break;
    case 0x5a:
      ld_r_r(&regs.E, &regs.D);
      break;
    case 0x5b:
      ld_r_r(&regs.E, &regs.E);
      break;
    case 0x5c:
      ld_r_r(&regs.E, &regs.H);
      break;
    case 0x5d:
      ld_r_r(&regs.E, &regs.L);
      break;
    case 0x5e:
      ld_r_indr(&regs.E, regs.HL, ram);
      break;
    case 0x5f:
      ld_r_r(&regs.E, &regs.A);
      break;
    case 0x60:
      ld_r_r(&regs.H, &regs.B);
      break;
    case 0x61:
      ld_r_r(&regs.H, &regs.C);
      break;
    case 0x62:
      ld_r_r(&regs.H, &regs.D);
      break;
    case 0x63:
      ld_r_r(&regs.H, &regs.E);
      break;
    case 0x64:
      ld_r_r(&regs.H, &regs.H);
      break;
    case 0x65:
      ld_r_r(&regs.H, &regs.L);
      break;
    case 0x66:
      ld_r_indr(&regs.H, regs.HL, ram);
      break;
    case 0x67:
      ld_r_r(&regs.H, &regs.A);
      break;
    case 0x68:
      ld_r_r(&regs.L, &regs.B);
      break;
    case 0x69:
      ld_r_r(&regs.L, &regs.C);
      break;
    case 0x6a:
      ld_r_r(&regs.L, &regs.D);
      break;
    case 0x6b:
      ld_r_r(&regs.L, &regs.E);
      break;
    case 0x6c:
      ld_r_r(&regs.L, &regs.H);
      break;
    case 0x6d:
      ld_r_r(&regs.L, &regs.L);
      break;
    case 0x6e:
      ld_r_indr(&regs.L, regs.HL, ram);
      break;
    case 0x6f:
      ld_r_r(&regs.L, &regs.A);
      break;
    case 0x70:
      ld_indr_r(regs.HL, &regs.B, ram);
      break;
    case 0x71:
      ld_indr_r(regs.HL, &regs.C, ram);
      break;
    case 0x72:
      ld_indr_r(regs.HL, &regs.D, ram);
      break;
    case 0x73:
      ld_indr_r(regs.HL, &regs.E, ram);
      break;
    case 0x74:
      ld_indr_r(regs.HL, &regs.H, ram);
      break;
    case 0x75:
      ld_indr_r(regs.HL, &regs.L, ram);
      break;
    case 0x76:
      halt();
      break;
    case 0x77:
      ld_indr_r(regs.HL, &regs.A, ram);
      break;
    case 0x78:
      ld_r_r(&regs.A, &regs.B);
      break;
    case 0x79:
      ld_r_r(&regs.A, &regs.C);
      break;
    case 0x7a:
      ld_r_r(&regs.A, &regs.D);
      break;
    case 0x7b:
      ld_r_r(&regs.A, &regs.E);
      break;
    case 0x7c:
      ld_r_r(&regs.A, &regs.H);
      break;
    case 0x7d:
      ld_r_r(&regs.A, &regs.L);
      break;
    case 0x7e:
      ld_r_indr(&regs.A, regs.HL, ram);
      break;
    case 0x7f:
      ld_r_r(&regs.A, &regs.A);
      break;
    case 0x80:
      add_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0x81:
      add_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0x82:
      add_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0x83:
      add_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0x84:
      add_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0x85:
      add_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0x86:
      add_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0x87:
      add_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0x88:
      adc_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0x89:
      adc_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0x8a:
      adc_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0x8b:
      adc_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0x8c:
      adc_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0x8d:
      adc_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0x8e:
      adc_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0x8f:
      adc_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0x90:
      sub_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0x91:
      sub_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0x92:
      sub_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0x93:
      sub_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0x94:
      sub_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0x95:
      sub_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0x96:
      sub_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0x97:
      sub_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0x98:
      sbc_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0x99:
      sbc_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0x9a:
      sbc_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0x9b:
      sbc_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0x9c:
      sbc_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0x9d:
      sbc_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0x9e:
      sbc_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0x9f:
      sbc_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0xa0:
      and_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0xa1:
      and_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0xa2:
      and_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0xa3:
      and_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0xa4:
      and_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0xa5:
      and_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0xa6:
      and_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0xa7:
      and_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0xa8:
      xor_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0xa9:
      xor_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0xaa:
      xor_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0xab:
      xor_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0xac:
      xor_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0xad:
      xor_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0xae:
      xor_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0xaf:
      xor_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0xb0:
      or_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0xb1:
      or_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0xb2:
      or_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0xb3:
      or_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0xb4:
      or_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0xb5:
      or_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0xb6:
      or_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0xb7:
      or_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0xb8:
      cp_r_r(&regs.A, &regs.B, &regs.F);
      break;
    case 0xb9:
      cp_r_r(&regs.A, &regs.C, &regs.F);
      break;
    case 0xba:
      cp_r_r(&regs.A, &regs.D, &regs.F);
      break;
    case 0xbb:
      cp_r_r(&regs.A, &regs.E, &regs.F);
      break;
    case 0xbc:
      cp_r_r(&regs.A, &regs.H, &regs.F);
      break;
    case 0xbd:
      cp_r_r(&regs.A, &regs.L, &regs.F);
      break;
    case 0xbe:
      cp_r_indr(&regs.A, regs.HL, &regs.F, ram);
      break;
    case 0xbf:
      cp_r_r(&regs.A, &regs.A, &regs.F);
      break;
    case 0xc0:
      ret_nz(&regs, ram);
      break;
    case 0xc1:
      pop_rr(&regs.BC, &regs.SP, ram);
      break;
    case 0xc2:
      jp_nz(&regs.F, (fetch_imm_nn(&regs.PC, ram)), &regs.PC);
      break;
    case 0xc3:
      jp(fetch_imm_nn(&regs.PC, ram), &regs.PC);
      break;
    case 0xc4:
      call_nz(&regs, (fetch_imm_nn(&regs.PC, ram)), ram);
      break;
    case 0xc5:
      push_rr(&regs.BC, &regs.SP, ram);
      break;
    case 0xc6:
      add_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xc7:
      rst(&regs, ram, 0x00);
      break;
    case 0xc8:
      ret_z(&regs, ram);
      break;
    case 0xc9:
      ret(&regs, ram);
      break;
    case 0xca:
      jp_z(&regs.F, fetch_imm_nn(&regs.PC, ram), &regs.PC);
      break;
    case 0xcb:
      ex_cb_instr(&regs, fetch_imm_n(&regs.PC, ram), ram);
      break;
    case 0xcc:
      call_nc(&regs, (fetch_imm_nn(&regs.PC, ram)), ram);
      break;
    case 0xcd:
      call(&regs, (fetch_imm_nn(&regs.PC, ram)), ram);
      break;
    case 0xce:
      adc_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xcf:
      rst(&regs, ram, 0x08);
      break;
    case 0xd0:
      ret_nc(&regs, ram);
      break;
    case 0xd1:
      pop_rr(&regs.DE, &regs.SP, ram);
      break;
    case 0xd2:
      jp_nc(&regs.F, (fetch_imm_nn(&regs.PC, ram)), &regs.PC);
      break;
    case 0xd3:
      // noop
      break;
    case 0xd4:
      call_nc(&regs, (fetch_imm_nn(&regs.PC, ram)), ram);
      break;
    case 0xd5:
      push_rr(&regs.DE, &regs.SP, ram);
      break;
    case 0xd6:
      sub_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xd7:
      rst(&regs, ram, 0x10);
      break;
    case 0xd8:
      ret_c(&regs, ram);
      break;
    case 0xd9:
      reti(&regs, ram, &ctx);
      break;
    case 0xda:
      jp_c(&regs.F, (fetch_imm_nn(&regs.PC, ram)), &regs.PC);
      break;
    case 0xdb:
      // noop
      break;
    case 0xdc:
      call_c(&regs, (fetch_imm_nn(&regs.PC, ram)), ram);
      break;
    case 0xdd:
      // noop
      break;
    case 0xde:
      adc_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xdf:
      rst(&regs, ram, 0x18);
      break;
    case 0xe0:
      ldh_indr_r(fetch_imm_n(&regs.PC, ram), &regs.A, ram);
      break;
    case 0xe1:
      pop_rr(&regs.HL, &regs.SP, ram);
      break;
    case 0xe2:
      ldh_indr_r(read(regs.C, ram), &regs.A, ram);
      break;
    case 0xe3:
      // noop
      break;
    case 0xe4:
      // noop
      break;
    case 0xe5:
      push_rr(&regs.HL, &regs.SP, ram);
      break;
    case 0xe6:
      and_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xe7:
      rst(&regs, ram, 0x20);
      break;
    case 0xe8:
      add_rr_n(&regs.SP, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xe9:
      jp_rr(&regs.HL, &regs.PC);
      break;
    case 0xea:
      ld_indr_r(fetch_imm_nn(&regs.PC, ram), &regs.A, ram);
      break;
    case 0xeb:
    case 0xec:
    case 0xed:
      // undefined
      break;
    case 0xee:
      xor_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xef:
      rst(&regs, ram, 0x28);
      break;
    case 0xf0:
      ldh_r_indr(&regs.A, fetch_imm_n(&regs.PC, ram), ram);
      break;
    case 0xf1:
      pop_rr(&regs.AF, &regs.SP, ram);
      break;
    case 0xf2:
      ld_r_indr(&regs.A, regs.C, ram);
      break;
    case 0xf3:
      di(&ctx);
      break;
    case 0xf4:
      // undefined
      break;
    case 0xf5:
      push_rr(&regs.AF, &regs.SP, ram);
      break;
    case 0xf6:
      or_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xf7:
      rst(&regs, ram, 0x30);
      break;
    case 0xf8:
      ld_hl_sp_dd(&regs, fetch_imm_n(&regs.PC, ram), ram);
      break;
    case 0xf9:
      ld_rr_rr(&regs.SP, &regs.HL);
      break;
    case 0xfa:
      ld_r_indr(&regs.A, fetch_imm_nn(&regs.PC, ram), ram);
      break;
    case 0xfb:
      ei(&ctx);
      break;
    case 0xfc:
    case 0xfd:
      // undefined
      break;
    case 0xfe:
      cp_r_n(&regs.A, fetch_imm_n(&regs.PC, ram), &regs.F);
      break;
    case 0xff:
      rst(&regs, ram, 0x38);
      break;
    }
  }
  return;
}

void ex_cb_instr(registers *regs, uint8_t op, uint8_t **ram) {
  uint8_t r = (op & 0x01) % 0x08;
  uint8_t reg;
  uint8_t arg = (0b00111000 & op) >> 3;
  switch (r) {
  case 0x00:
    reg = regs->B;
    break;
  case 0x01:
    reg = regs->C;
    break;
  case 0x02:
    reg = regs->D;
    break;
  case 0x03:
    reg = regs->E;
    break;
  case 0x04:
    reg = regs->H;
    break;
  case 0x05:
    reg = regs->L;
    break;
  case 0x07:
    reg = regs->A;
    break;
  }

  // clang-format off
  switch (op) {
  case 0x00:  case 0x01:  case 0x02:  case 0x03:  case 0x04:  case 0x05:  case 0x07:
      rlc(&reg, &regs->F);
    break;
  case 0x06:
    rlc_indr(regs->HL, &regs->F, ram);
    break;
  case 0x08:  case 0x09:  case 0x0a:  case 0x0b:  case 0x0c:  case 0x0d:  case 0x0f:
    rrc(&reg, &regs->F);
    break;
  case 0x0e:
    rrc_indr(regs->HL, &regs->F, ram);
    break;
  case 0x10:  case 0x11:  case 0x12:  case 0x13:  case 0x14:  case 0x15:  case 0x17:
    rl(&regs->A, &regs->F);
    break;
  case 0x16:
    rl_indr(regs->HL, &regs->F, ram);
    break;
  case 0x18:  case 0x19:  case 0x1a:  case 0x1b:  case 0x1c:  case 0x1d:  case 0x1f:
    rr(&regs->A, &regs->F);
    break;
  case 0x1e:
    rr_indr(regs->HL, &regs->F, ram);
    break;
  case 0x20:  case 0x21:  case 0x22:  case 0x23:  case 0x24:  case 0x25:  case 0x27:
    sla(&regs->A, &regs->F);
    break;
  case 0x26:
    sla_indr(regs->HL, &regs->F, ram);
    break;
  case 0x28:  case 0x29:  case 0x2a:  case 0x2b:  case 0x2c:  case 0x2d:  case 0x2f:
    sra(&regs->A, &regs->F);
    break;
  case 0x2e:
    sra_indr(regs->HL, &regs->F, ram);
    break;
  case 0x30:  case 0x31:  case 0x32:  case 0x33:  case 0x34:  case 0x35:  case 0x37:
    swap(&regs->A, &regs->F);
    break;
  case 0x36:
    swap_indr(regs->HL, &regs->F, ram);
    break;
  case 0x38:  case 0x39:  case 0x3a:  case 0x3b:  case 0x3c:  case 0x3d:  case 0x3f:
    srl(&regs->A, &regs->F);
    break;
  case 0x3e:
    srl_indr(regs->HL, &regs->F, ram);
    break;
  case 0x40:  case 0x41:  case 0x42:  case 0x43:  case 0x44:  case 0x45:  case 0x47:
  case 0x48:  case 0x49:  case 0x4a:  case 0x4b:  case 0x4c:  case 0x4d:  case 0x4f:
  case 0x50:  case 0x51:  case 0x52:  case 0x53:  case 0x54:  case 0x55:  case 0x57:
  case 0x58:  case 0x59:  case 0x5a:  case 0x5b:  case 0x5c:  case 0x5d:  case 0x5f:
  case 0x60:  case 0x61:  case 0x62:  case 0x63:  case 0x64:  case 0x65:  case 0x67:
  case 0x68:  case 0x69:  case 0x6a:  case 0x6b:  case 0x6c:  case 0x6d:  case 0x6f:
  case 0x70:  case 0x71:  case 0x72:  case 0x73:  case 0x74:  case 0x75:  case 0x77:
  case 0x78:  case 0x79:  case 0x7a:  case 0x7b:  case 0x7c:  case 0x7d:  case 0x7f:
    bit(arg, &reg, &regs->F);
    break;
  case 0x46:  case 0x4e:  
  case 0x56:  case 0x5e:  
  case 0x66:  case 0x6e:  
  case 0x76:  case 0x7e:  
    bit_indr(arg, regs->HL, &regs->F, ram);
    break;
  case 0x80:  case 0x81:  case 0x82:  case 0x83:  case 0x84:  case 0x85:  case 0x87:
  case 0x88:  case 0x89:  case 0x8a:  case 0x8b:  case 0x8c:  case 0x8d:  case 0x8f:
  case 0x90:  case 0x91:  case 0x92:  case 0x93:  case 0x94:  case 0x95:  case 0x97:
  case 0x98:  case 0x99:  case 0x9a:  case 0x9b:  case 0x9c:  case 0x9d:  case 0x9f:
  case 0xa0:  case 0xa1:  case 0xa2:  case 0xa3:  case 0xa4:  case 0xa5:  case 0xa7:
  case 0xa8:  case 0xa9:  case 0xaa:  case 0xab:  case 0xac:  case 0xad:  case 0xaf:
  case 0xb0:  case 0xb1:  case 0xb2:  case 0xb3:  case 0xb4:  case 0xb5:  case 0xb7:
  case 0xb8:  case 0xb9:  case 0xba:  case 0xbb:  case 0xbc:  case 0xbd:  case 0xbf:
    res(arg, &reg, &regs->F);
    break;
  case 0x86:  case 0x8e:  
  case 0x96:  case 0x9e:  
  case 0xa6:  case 0xae:  
  case 0xb6:  case 0xbe:  
    res_indr(arg, regs->HL, &regs->F, ram);
    break;
  case 0xc0:  case 0xc1:  case 0xc2:  case 0xc3:  case 0xc4:  case 0xc5:  case 0xc7:
  case 0xc8:  case 0xc9:  case 0xca:  case 0xcb:  case 0xcc:  case 0xcd:  case 0xcf:
  case 0xd0:  case 0xd1:  case 0xd2:  case 0xd3:  case 0xd4:  case 0xd5:  case 0xd7:
  case 0xd8:  case 0xd9:  case 0xda:  case 0xdb:  case 0xdc:  case 0xdd:  case 0xdf:
  case 0xe0:  case 0xe1:  case 0xe2:  case 0xe3:  case 0xe4:  case 0xe5:  case 0xe7:
  case 0xe8:  case 0xe9:  case 0xea:  case 0xeb:  case 0xec:  case 0xed:  case 0xef:
  case 0xf0:  case 0xf1:  case 0xf2:  case 0xf3:  case 0xf4:  case 0xf5:  case 0xf7:
  case 0xf8:  case 0xf9:  case 0xfa:  case 0xfb:  case 0xfc:  case 0xfd:  case 0xff:
    set(arg, &reg, &regs->F);
    break;
  case 0xc6:  case 0xce:  
  case 0xd6:  case 0xde:  
  case 0xe6:  case 0xee:  
  case 0xf6:  case 0xfe:  
    set_indr(arg, regs->HL, &regs->F, ram);
    break;
  }
  // clang-format on
}

// load instructions

void ld_r_r(uint8_t *reg1, uint8_t *reg2) { (*reg1) = (*reg2); }

void ld_r_n(uint8_t *reg, uint8_t n) { (*reg) = n; }

void ld_indr_r(uint16_t addr, uint8_t *reg, uint8_t **ram) {
  write(addr, ram, (*reg));
}

void ld_indr_rr(uint16_t addr, uint16_t *reg, uint8_t **ram) {
  write(addr, ram, (*reg));
}

void ld_indr_nn(uint16_t addr, uint16_t n, uint8_t **ram) {
  write(addr, ram, n);
}

void ld_r_indr(uint8_t *reg, uint16_t addr, uint8_t **ram) {
  (*reg) = read(addr, ram);
}

void ld_rr_rr(uint16_t *reg1, uint16_t *reg2) { (*reg1) = (*reg2); }

void ld_rr_nn(uint16_t *reg1, uint16_t nn) { (*reg1) = nn; }

void ldh_r_indr(uint8_t *reg, uint8_t addr, uint8_t **ram) {
  (*reg) = read(0xFF00 + addr, ram);
}
void ldh_indr_r(uint8_t addr, uint8_t *reg, uint8_t **ram) {
  write(0xFF00 + addr, ram, (*reg));
}

void ld_hl_sp_dd(registers *regs, uint8_t dd, uint8_t **ram) {
  uint16_t prev = regs->SP;
  int8_t signed_dd = (int8_t)dd;
  uint16_t addr = (regs->SP) + signed_dd;

  if (signed_dd >= 0) {
    chk_carry_r(prev, addr, &regs->F);
  } else {
    if ((prev & 0x80) == 0 && (addr & 0x80) == 1) {
      set_f(&regs->F, FLAG_MASK_C);
    } else {
      clear_f(&regs->F, FLAG_MASK_C);
    }

    if ((prev & 0x08) == 0 && (addr & 0x8) == 1) {
      set_f(&regs->F, FLAG_MASK_H);
    } else {
      clear_f(&regs->F, FLAG_MASK_H);
    }
  }

  regs->HL = read(addr, ram);
  clear_f(&regs->F, FLAG_MASK_N);
  clear_f(&regs->F, FLAG_MASK_Z);
}

void push_rr(uint16_t *reg, uint16_t *sp, uint8_t **ram) {
  write(--(*sp), ram, ((*reg) & 0xFF00) >> 8);
  write(--(*sp), ram, ((*reg) & 0x00FF));
}

void pop_rr(uint16_t *reg, uint16_t *sp, uint8_t **ram) {
  uint8_t lsb = read((*sp)++, ram);
  uint8_t msb = read((*sp)++, ram);
  (*reg) = (msb << 8) | lsb;
}

// arithmetic instructions
// inc
void inc_rr(uint16_t *reg) { (*reg)++; }

void inc_r(uint8_t *reg, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg)++;
  chk_carry_r(prev, (*reg), f);
}

void inc_indr(uint16_t addr, uint8_t *f, uint8_t **ram) { (*ram)[addr]++; }

// dec
void dec_rr(uint16_t *reg) { (*reg)--; }

void dec_r(uint8_t *reg, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg)--;
  chk_carry_r(prev, (*reg), f);
}

void dec_indr(uint16_t addr, uint8_t *f, uint8_t **ram) { (*ram)[addr]--; }

// add
void add_rr_rr(uint16_t *reg1, uint16_t *reg2, uint8_t *f) {
  uint16_t prev = (*reg1);
  (*reg1) += (*reg2);
  clear_f(f, FLAG_MASK_N);
  chk_carry_rr(prev, (*reg1), f);
}

void add_rr_n(uint16_t *reg, uint8_t n, uint8_t *f) {
  uint16_t prev = (*reg);
  (*reg) += n;
  clear_f(f, FLAG_MASK_N);
  clear_f(f, FLAG_MASK_Z);
  chk_carry_rr(prev, (*reg), f);
}

void add_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  uint8_t prev = (*reg1);
  (*reg1) += (*reg2);
  chk_zero((*reg1), f);
  clear_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg1), f);
}

void add_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t prev = (*reg);
  (*reg) += (*ram)[addr];
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg), f);
}

void add_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg) += n;
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg), f);
}

// adc
void adc_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg) += n + (((*f) & FLAG_MASK_C) >> 3);
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg), f);
}

void adc_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  adc_r_n(reg1, (*reg2), f);
}

void adc_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  adc_r_n(reg, (*ram)[addr], f);
}

// sub
void sub_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg) -= n;
  chk_zero((*reg), f);
  set_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg), f);
}

void sub_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  sub_r_n(reg1, (*reg2), f);
}

void sub_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  sub_r_n(reg, (*ram)[addr], f);
}

// sbc
void sbc_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  sbc_r_n(reg1, (*reg2), f);
}

void sbc_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  sbc_r_n(reg, (*ram)[addr], f);
}

void sbc_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  uint8_t prev = (*reg);
  (*reg) -= n - (((*f) & FLAG_MASK_C) >> 3);
  chk_zero((*reg), f);
  set_f(f, FLAG_MASK_N);
  chk_carry_r(prev, (*reg), f);
}

// and
void and_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  and_r_n(reg1, (*reg2), f);
}
void and_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  and_r_n(reg, (*ram)[addr], f);
}
void and_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  (*reg) &= n;
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  set_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_C);
}

void xor_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  xor_r_n(reg1, (*reg2), f);
}
void xor_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  xor_r_n(reg, (*ram)[addr], f);
}
void xor_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  (*reg) ^= n;
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_C);
}

void or_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  or_r_n(reg1, (*reg2), f);
}
void or_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  or_r_n(reg, (*ram)[addr], f);
}
void or_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  (*reg) |= n;
  chk_zero((*reg), f);
  clear_f(f, FLAG_MASK_N);
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_C);
}

// cp
void cp_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f) {
  (or_r_n(reg1, (*reg2), f));
}

void cp_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram) {
  (or_r_n(reg, (*ram)[addr], f));
}

void cp_r_n(uint8_t *reg, uint8_t n, uint8_t *f) {
  uint8_t res = (*reg) - n;
  chk_zero(res, f);
  set_f(f, FLAG_MASK_N);
  chk_carry_r(res, (*reg), f);
}

void daa(uint8_t *a, uint8_t *flags) {
  if (((*flags) | FLAG_MASK_N) > 0) {
    if (((*flags) | FLAG_MASK_C) > 0 || (*a) > 0x90) {
      (*a) += 0x60;
      set_f(flags, FLAG_MASK_C);
    }
    if (((*flags) | FLAG_MASK_H) > 0 || ((*a) & 0x0F) > 0x06) {
      (*a) += 0x06;
    }
  } else {
    if (((*flags) | FLAG_MASK_C) > 0) {
      (*a) -= 0x60;
    }
    if (((*flags) | FLAG_MASK_H) > 0) {
      (*a) -= 0x06;
    }
  }
  chk_zero((*a), flags);
  clear_f(flags, FLAG_MASK_H);
}

// bit manipulation
// rotate left circular
uint8_t rlc_n(uint8_t n, uint8_t *f) {
  uint8_t cy = ((n & BIT_MASK_7) >> 7);
  n = (n << 1) | cy;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  return n;
}

void rlc(uint8_t *reg, uint8_t *f) {
  uint8_t res = rlc_n((*reg), f);
  (*reg) = res;
  chk_zero(res, f);
}

void rlc_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t res = rlc_n((*ram)[addr], f);
  (*ram)[addr] = res;
  chk_zero(res, f);
}

void rlca(uint8_t *reg, uint8_t *f) {
  (*reg) = rlc_n((*reg), f);
  clear_f(f, FLAG_MASK_Z);
}

// rotate left through carry
uint8_t rl_n(uint8_t n, uint8_t *f) {
  uint8_t cy = (n & BIT_MASK_7);
  n = n << 1;
  n |= (FLAG_MASK_C & (*f)) >> 3;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  return n;
}

void rl(uint8_t *reg, uint8_t *f) {
  uint8_t res = rl_n((*reg), f);
  (*reg) = res;
  chk_zero(res, f);
}

void rl_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t res = rl_n((*ram)[addr], f);
  (*ram)[addr] = res;
  chk_zero(res, f);
}

void rla(uint8_t *reg, uint8_t *f) {
  uint8_t res = rl_n((*reg), f);
  clear_f(f, FLAG_MASK_Z);
}

// rotate right circular
uint8_t rrc_n(uint8_t n, uint8_t *f) {
  uint8_t cy = (n & 0x01) << 7;
  n = (n >> 1) | cy;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  return n;
}

void rrc(uint8_t *reg, uint8_t *f) {
  uint8_t res = rrc_n((*reg), f);
  (*reg) = res;
  chk_zero(res, f);
}

void rrc_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t res = rrc_n((*ram)[addr], f);
  (*ram)[addr] = res;
  chk_zero(res, f);
}

void rrca(uint8_t *reg, uint8_t *f) {
  (*reg) = rrc_n((*reg), f);
  clear_f(f, FLAG_MASK_Z);
}

// rotate right through carry
uint8_t rr_n(uint8_t n, uint8_t *f) {
  uint8_t cy = (n & 0x01);
  n = n >> 1;
  n |= (FLAG_MASK_C & (*f)) << 4;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  return n;
}

void rr(uint8_t *reg, uint8_t *f) {
  uint8_t res = rr_n((*reg), f);
  (*reg) = res;
  chk_zero(res, f);
}

void rr_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t res = rr_n((*ram)[addr], f);
  (*ram)[addr] = res;
  chk_zero(res, f);
}

void rra(uint8_t *reg, uint8_t *f) {
  (*reg) = rr_n((*reg), f);
  clear_f(f, FLAG_MASK_Z);
}

// arithmetic shifts
// left shifts are identical to logical ones
uint8_t sla_n(uint8_t n, uint8_t *f) {
  uint8_t cy = n & 0x01;
  n = n << 1;
  n |= cy;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  chk_zero(n, f);
  return n;
}

void sla(uint8_t *reg, uint8_t *f) { (*reg) = rlc_n((*reg), f); }

void sla_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  (*ram)[addr] = rlc_n((*ram)[addr], f);
}

uint8_t sra_n(uint8_t n, uint8_t *f) {
  uint8_t cy = n & BIT_MASK_7;
  n = n >> 1;
  n |= cy;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  chk_zero(n, f);
  return n;
}

void sra(uint8_t *reg, uint8_t *f) { (*reg) = sra_n((*reg), f); }

void sra_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  (*ram)[addr] = sra_n((*ram)[addr], f);
}

// swap nibbles
uint8_t swap_n(uint8_t n, uint8_t *f) {
  uint8_t temp = n >> 4;
  n &= (n << 4) & temp;
  clear_f(f, FLAG_MASK_C);
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  chk_zero(n, f);
  return n;
}
void swap(uint8_t *reg, uint8_t *f) { (*reg) = swap_n((*reg), f); }

void swap_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {
  (*ram)[addr] = swap_n((*ram)[addr], f);
}

// logical shift
uint8_t srl_n(uint8_t n, uint8_t *f) {
  uint8_t cy = n & BIT_MASK_7;
  n = n >> 1;
  if (cy >= 1) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }
  clear_f(f, FLAG_MASK_H);
  clear_f(f, FLAG_MASK_N);
  chk_zero(n, f);
  return n;
}

void srl(uint8_t *reg, uint8_t *f) { (*reg) = srl_n((*reg), f); }

void srl_indr(uint16_t addr, uint8_t *f, uint8_t **ram) {}

// bit manipulation
void bit(uint8_t n, uint8_t *reg, uint8_t *f) {
  uint8_t test = n & (0x1 << n);
  if (((*reg) & test) >= 1) {
    set_f(f, FLAG_MASK_Z);
  } else {
    clear_f(f, FLAG_MASK_Z);
  }
  set_f(f, FLAG_MASK_N);
  set_f(f, FLAG_MASK_H);
}

void bit_indr(uint8_t n, uint16_t addr, uint8_t *f, uint8_t **ram) {
  uint8_t test = n & (0x1 << n);
  if (((*ram)[addr] & test) >= 1) {
    set_f(f, FLAG_MASK_Z);
  } else {
    clear_f(f, FLAG_MASK_Z);
  }
  set_f(f, FLAG_MASK_N);
  set_f(f, FLAG_MASK_H);
}

void res(uint8_t n, uint8_t *reg, uint8_t *f) { (*reg) &= ~(0x1 << n); }

void res_indr(uint8_t n, uint16_t addr, uint8_t *f, uint8_t **ram) {
  (*ram)[addr] &= ~(0x1 << n);
}

void set(uint8_t n, uint8_t *reg, uint8_t *f) { (*reg) |= (0x1 << n); }

void set_indr(uint8_t n, uint16_t addr, uint8_t *f, uint8_t **ram) {
  (*ram)[addr] |= (0x1 << n);
}

// jumps
// relative
void jr(uint16_t *pc, uint8_t off) { (*pc) += off; }
void jr_nz(uint8_t *f, uint16_t *pc, uint8_t off) {
  if (((*f) & FLAG_MASK_Z) > 0) {
    (*pc) += (int8_t)off;
  }
}
void jr_z(uint8_t *f, uint16_t *pc, uint8_t off) {
  if (((*f) & FLAG_MASK_Z) == 0) {
    (*pc) += (int8_t)off;
  }
}
void jr_nc(uint8_t *f, uint16_t *pc, uint8_t off) {
  if (((*f) & FLAG_MASK_C) > 0) {
    (*pc) += (int8_t)off;
  }
}

void jr_c(uint8_t *f, uint16_t *pc, uint8_t off) {
  if (((*f) & FLAG_MASK_C) == 0) {
    (*pc) += (int8_t)off;
  }
}

// absolute
void jp(uint16_t addr, uint16_t *pc) { (*pc) = addr; }

void jp_rr(uint16_t *reg, uint16_t *pc) { (*pc) = (*reg); }

void jp_c(uint8_t *f, uint16_t addr, uint16_t *pc) {
  if (((*f) & FLAG_MASK_C) > 0) {
    (*pc) = addr;
  }
}

void jp_z(uint8_t *f, uint16_t addr, uint16_t *pc) {
  if (((*f) & FLAG_MASK_Z) > 0) {
    (*pc) = addr;
  }
}

void jp_nc(uint8_t *f, uint16_t addr, uint16_t *pc) {
  if (((*f) & FLAG_MASK_C) == 0) {
    (*pc) = addr;
  }
}
void jp_nz(uint8_t *f, uint16_t addr, uint16_t *pc) {
  if (((*f) & FLAG_MASK_Z) == 0) {
    (*pc) = addr;
  }
}

// returns
void ret(registers *regs, uint8_t **ram) {
  uint8_t lsb = read(regs->SP--, ram);
  uint8_t msb = read(regs->SP--, ram);
  regs->PC = (msb << 8) | lsb;
}
void ret_nz(registers *regs, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_Z) > 0) {
    ret(regs, ram);
  }
}
void ret_z(registers *regs, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_Z) == 0) {
    ret(regs, ram);
  }
}
void ret_nc(registers *regs, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_C) > 0) {
    ret(regs, ram);
  }
}
void ret_c(registers *regs, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_C) == 0) {
    ret(regs, ram);
  }
}
void reti(registers *regs, uint8_t **ram, context *ctx) {
  ret(regs, ram);
  ctx->IME = true;
}

// calls
void call(registers *regs, uint16_t addr, uint8_t **ram) {
  (regs->SP--);
  (*ram)[regs->SP--] = (0xFF00 & regs->PC) >> 8;
  (*ram)[regs->SP--] = (0x00FF & regs->PC);
  regs->PC = addr;
}
void call_nz(registers *regs, uint16_t addr, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_Z) > 0) {
    call(regs, addr, ram);
  }
}
void call_z(registers *regs, uint16_t addr, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_Z) == 0) {
    call(regs, addr, ram);
  }
}
void call_nc(registers *regs, uint16_t addr, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_C) > 0) {
    call(regs, addr, ram);
  }
}
void call_c(registers *regs, uint16_t addr, uint8_t **ram) {
  if ((regs->F & FLAG_MASK_C) == 0) {
    call(regs, addr, ram);
  }
}

void rst(registers *regs, uint8_t **ram, uint16_t rstv) {
  call(regs, rstv, ram);
}

// misc
void stop(uint8_t n) {}
void halt() {}
void ei(context *ctx) { ctx->IME = 1; }
void di(context *ctx) { ctx->IME = 1; }

void cpl(uint8_t *a, uint8_t *f) {
  (*a) = ~(*a);
  set_f(f, FLAG_MASK_N);
  set_f(f, FLAG_MASK_H);
}
void scf(uint8_t *f) {
  set_f(f, FLAG_MASK_C);
  set_f(f, FLAG_MASK_N);
  set_f(f, FLAG_MASK_H);
}
void ccf(uint8_t *f) {
  (*f) = (*f) & ~((*f) & FLAG_MASK_C);
  clear_f(f, FLAG_MASK_N);
  clear_f(f, FLAG_MASK_H);
}

void chk_carry_rr(uint16_t prev, uint16_t calc, uint8_t *f) {
  if ((prev & BIT_MASK_15) == 1 && (calc & BIT_MASK_15) == 0) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }

  if ((prev & BIT_MASK_11) == 1 && (calc & BIT_MASK_11) == 0) {
    set_f(f, FLAG_MASK_H);
  } else {
    clear_f(f, FLAG_MASK_H);
  }
}

void chk_carry_r(uint8_t prev, uint8_t calc, uint8_t *f) {
  if ((prev & BIT_MASK_7) == 1 && (calc & BIT_MASK_7) == 0) {
    set_f(f, FLAG_MASK_C);
  } else {
    clear_f(f, FLAG_MASK_C);
  }

  if ((prev & BIT_MASK_3) == 1 && (calc & BIT_MASK_3) == 0) {
    set_f(f, FLAG_MASK_H);
  } else {
    clear_f(f, FLAG_MASK_H);
  }
}

void chk_zero(uint16_t val, uint8_t *f) {
  if (val == 0) {
    set_f(f, FLAG_MASK_Z);
  } else {
    clear_f(f, FLAG_MASK_Z);
  }
}

uint16_t fetch_imm_nn(uint16_t *pc, uint8_t **ram) {
  uint8_t lsb = fetch_imm_n(pc, ram);
  uint8_t msb = fetch_imm_n(pc, ram);
  return (msb << 8) | lsb;
}

uint8_t fetch_imm_n(uint16_t *pc, uint8_t **ram) { return read((*pc)++, ram); }

uint8_t read(uint16_t addr, uint8_t **ram) { return (*ram)[addr]; }

void write(uint16_t addr, uint8_t **ram, uint8_t val) { (*ram)[addr] = val; }

void set_f(uint8_t *flag, uint8_t mask) { (*flag) |= mask; }

void clear_f(uint8_t *flag, uint8_t mask) { (*flag) &= ~mask; }
