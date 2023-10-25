#ifndef _ASM_OFFSETS_H
#define _ASM_OFFSETS_H

/* LoongArch pt_regs offsets. */
#define PT_R0 0 /* offsetof(struct pt_regs, regs[0]) */
#define PT_R1 8 /* offsetof(struct pt_regs, regs[1]) */
#define PT_R2 16 /* offsetof(struct pt_regs, regs[2]) */
#define PT_R3 24 /* offsetof(struct pt_regs, regs[3]) */
#define PT_R4 32 /* offsetof(struct pt_regs, regs[4]) */
#define PT_R5 40 /* offsetof(struct pt_regs, regs[5]) */
#define PT_R6 48 /* offsetof(struct pt_regs, regs[6]) */
#define PT_R7 56 /* offsetof(struct pt_regs, regs[7]) */
#define PT_R8 64 /* offsetof(struct pt_regs, regs[8]) */
#define PT_R9 72 /* offsetof(struct pt_regs, regs[9]) */
#define PT_R10 80 /* offsetof(struct pt_regs, regs[10]) */
#define PT_R11 88 /* offsetof(struct pt_regs, regs[11]) */
#define PT_R12 96 /* offsetof(struct pt_regs, regs[12]) */
#define PT_R13 104 /* offsetof(struct pt_regs, regs[13]) */
#define PT_R14 112 /* offsetof(struct pt_regs, regs[14]) */
#define PT_R15 120 /* offsetof(struct pt_regs, regs[15]) */
#define PT_R16 128 /* offsetof(struct pt_regs, regs[16]) */
#define PT_R17 136 /* offsetof(struct pt_regs, regs[17]) */
#define PT_R18 144 /* offsetof(struct pt_regs, regs[18]) */
#define PT_R19 152 /* offsetof(struct pt_regs, regs[19]) */
#define PT_R20 160 /* offsetof(struct pt_regs, regs[20]) */
#define PT_R21 168 /* offsetof(struct pt_regs, regs[21]) */
#define PT_R22 176 /* offsetof(struct pt_regs, regs[22]) */
#define PT_R23 184 /* offsetof(struct pt_regs, regs[23]) */
#define PT_R24 192 /* offsetof(struct pt_regs, regs[24]) */
#define PT_R25 200 /* offsetof(struct pt_regs, regs[25]) */
#define PT_R26 208 /* offsetof(struct pt_regs, regs[26]) */
#define PT_R27 216 /* offsetof(struct pt_regs, regs[27]) */
#define PT_R28 224 /* offsetof(struct pt_regs, regs[28]) */
#define PT_R29 232 /* offsetof(struct pt_regs, regs[29]) */
#define PT_R30 240 /* offsetof(struct pt_regs, regs[30]) */
#define PT_R31 248 /* offsetof(struct pt_regs, regs[31]) */
#define PT_CRMD 280 /* offsetof(struct pt_regs, csr_crmd) */
#define PT_PRMD 288 /* offsetof(struct pt_regs, csr_prmd) */
#define PT_EUEN 296 /* offsetof(struct pt_regs, csr_euen) */
#define PT_ECFG 304 /* offsetof(struct pt_regs, csr_ecfg) */
#define PT_ESTAT 312 /* offsetof(struct pt_regs, csr_estat) */
#define PT_ERA 264 /* offsetof(struct pt_regs, csr_era) */
#define PT_BVADDR 272 /* offsetof(struct pt_regs, csr_badvaddr) */
#define PT_ORIG_A0 256 /* offsetof(struct pt_regs, orig_a0) */
#define PT_SIZE 320 /* sizeof(struct pt_regs) */

#endif /* _ASM_OFFSETS_H */
