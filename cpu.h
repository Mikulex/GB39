#ifndef CPU_HEADER_INCLUDED
#define CPU_HEADER_INCLUDED

#include <stdint.h>

typedef struct registers {
  union {
    uint16_t AF;
    struct {
      uint8_t A;
      uint8_t F;
    };
  };
  union {
    uint16_t BC;
    struct {
      uint8_t B;
      uint8_t C;
    };
  };
  union {
    uint16_t DE;
    struct {
      uint8_t D;
      uint8_t E;
    };
  };
  union {
    uint16_t HL;
    struct {
      uint8_t H;
      uint8_t L;
    };
  };
  uint16_t SP;
  uint16_t PC;
} registers;

// load instructions
void ld_r_r(uint8_t *reg1, uint8_t *reg2);
void ld_r_n(uint8_t *reg, uint8_t n);

void ld_indr_r(uint16_t addr, uint8_t *reg, uint8_t **ram);
void ld_indr_rr(uint16_t addr, uint16_t *reg, uint8_t **ram);

void ld_indr_nn(uint16_t addr, uint16_t n, uint8_t **ram);

void ld_r_indr(uint8_t *reg, uint16_t addr, uint8_t **ram);

void ld_rr_rr(uint16_t *reg1, uint16_t *reg2);
void ld_rr_nn(uint16_t *reg1, uint16_t nn);

void ldh_indr_r(uint8_t addr, uint8_t *reg, uint8_t **ram);
void ldh_r_indr(uint8_t *reg, uint8_t addr, uint8_t **ram);

void ld_hl_sp_dd(registers *regs, uint8_t dd, uint8_t **ram);

void push_rr(uint16_t *reg, uint16_t *sp, uint8_t **ram);
void pop_rr(uint16_t *reg, uint16_t *sp, uint8_t **ram);

// arithmetic instructions
void inc_rr(uint16_t *reg);
void inc_r(uint8_t *reg, uint8_t *f);
void inc_indr(uint16_t addr, uint8_t *f, uint8_t **ram);

void dec_rr(uint16_t *reg);
void dec_r(uint8_t *reg, uint8_t *f);
void dec_indr(uint16_t addr, uint8_t *f, uint8_t **ram);

void add_rr_rr(uint16_t *reg1, uint16_t *reg2, uint8_t *f);
void add_rr_n(uint16_t *reg, uint8_t n, uint8_t *f);
void add_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void add_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void add_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void adc_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void adc_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void adc_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void sub_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void sub_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void sub_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void sbc_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void sbc_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void sbc_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void and_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void and_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void and_r_n(uint8_t *reg, uint8_t n, uint8_t *f, uint8_t **ram);

void xor_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void xor_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void xor_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void or_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void or_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void or_n_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void or_r_n(uint8_t *reg, uint8_t n, uint8_t *f, uint8_t **ram);

void cp_r_r(uint8_t *reg1, uint8_t *reg2, uint8_t *f);
void cp_r_indr(uint8_t *reg, uint16_t addr, uint8_t *f, uint8_t **ram);
void cp_r_n(uint8_t *reg, uint8_t n, uint8_t *f);

void daa(uint8_t *a, uint8_t *flags);
void cpl(uint8_t *a, uint8_t *f);
void scf(uint8_t *f);
void ccf(uint8_t *f);

// bit manipulation instructions
void rlc(uint8_t *reg, uint8_t *f);
void rlc_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);
void rl(uint8_t *reg, uint8_t *f);
void rl_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);

void rrc(uint8_t *reg, uint8_t *f);
void rrc_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);
void rr(uint8_t *reg, uint8_t *f);
void rr_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);

void sla(uint8_t *reg, uint8_t *f);
void sla_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);
void sra(uint8_t *reg, uint8_t *f);
void sra_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);

void swap(uint8_t *reg, uint8_t *f);
void swap_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);
void srl(uint8_t *reg, uint8_t *f);
void srl_indr(uint16_t *reg, uint8_t *f, uint8_t **ram);

// single bit manipulations
void bit(uint8_t n, uint8_t *reg, uint8_t *flags);
void bit_indr(uint8_t n, uint16_t addr, uint8_t *flags, uint8_t **ram);

void res(uint8_t n, uint8_t *reg, uint8_t *flags);
void res_indr(uint8_t n, uint16_t addr, uint8_t *flags, uint8_t **ram);

void set(uint8_t n, uint8_t *reg, uint8_t *flags);
void set_indr(uint8_t n, uint16_t addr, uint8_t *flags, uint8_t **ram);

// jump instructions
void jr(uint16_t *pc, uint8_t off);
void jr_nz(uint8_t *f, uint16_t *pc, uint8_t off);
void jr_z(uint8_t *f, uint16_t *pc, uint8_t off);
void jr_nc(uint8_t *f, uint16_t *pc, uint8_t off);
void jr_c(uint8_t *f, uint16_t *pc, uint8_t off);

void jp(uint16_t addr, uint16_t *pc);
void jp_rr(uint16_t *reg, uint16_t *pc);
void jp_c(uint8_t *f, uint16_t addr, uint16_t *pc);
void jp_z(uint8_t *f, uint16_t addr, uint16_t *pc);
void jp_nc(uint8_t *f, uint16_t addr, uint16_t *pc);
void jp_nz(uint8_t *f, uint16_t addr, uint16_t *pc);

void ret(uint16_t *pc, uint16_t *sp);
void ret_nz(uint8_t *f, uint16_t *pc, uint16_t *sp);
void ret_z(uint8_t *f, uint16_t *pc, uint16_t *sp);
void ret_nc(uint8_t *f, uint16_t *pc, uint16_t *sp);
void ret_c(uint8_t *f, uint16_t *pc, uint16_t *sp);
void reti(uint8_t *f, uint16_t *pc, uint16_t *sp);

void call(uint16_t *pc, uint16_t *sp);
void call_nz(uint8_t *f, uint16_t addr, uint16_t *pc, uint16_t *sp);
void call_nc(uint8_t *f, uint16_t addr, uint16_t *pc, uint16_t *sp);
void call_c(uint8_t *f, uint16_t addr, uint16_t *pc, uint16_t *sp);
void call_z(uint8_t *f, uint16_t addr, uint16_t *pc, uint16_t *sp);

void rst(uint16_t *pc, uint16_t *sp, uint16_t rstv);

// misc instructions
void stop(uint8_t n);
void halt();
void ei(uint8_t *f);
void di(uint8_t *f);

// helpers
void tick(void);
void start(uint8_t **boot_rom, uint8_t **ram, uint8_t **vram);
uint8_t read(uint16_t addr, uint8_t **ram);
void write(uint16_t addr, uint8_t **ram, uint8_t val);
uint16_t fetch_imm_nn(uint16_t *pc, uint8_t **ram);
uint8_t fetch_imm_n(uint16_t *pc, uint8_t **ram);
void ex_cb_instr(registers *r, uint8_t op, uint8_t **ram);

void set_f(uint8_t *flag, uint8_t mask);
void clear_f(uint8_t *flag, uint8_t mask);
void chk_carry_r(uint8_t prev, uint8_t calc, uint8_t *f);
void chk_carry_rr(uint16_t prev, uint16_t calc, uint8_t *f);
void chk_zero(uint16_t val, uint8_t *f);

#endif
