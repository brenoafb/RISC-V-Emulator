#ifndef SIGNATURES_H
#define SIGNATURES_H

#define SIG(OP, F3, F7) ((uint64_t) (F7 << 15) + (F3 << 7) + OP)

#define S_ECALL (SIG(0x73, 0x0, 0x00))

#define S_ADD  (SIG(0x33, 0x0, 0x00))
#define S_SUB  (SIG(0x33, 0x0, 0x20))
#define S_XOR  (SIG(0x33, 0x4, 0x00))
#define S_OR   (SIG(0x33, 0x6, 0x00))
#define S_AND  (SIG(0x33, 0x7, 0x00))
#define S_SLL  (SIG(0x33, 0x1, 0x00))
#define S_SRL  (SIG(0x33, 0x5, 0x00))
#define S_SRA  (SIG(0x33, 0x5, 0x20))

#define S_ADDI   (SIG(0x13, 0x0, 0x00))
#define S_SLTI   (SIG(0x13, 0x2, 0x00))
#define S_SLTIU  (SIG(0x13, 0x3, 0x00))
#define S_XORI   (SIG(0x13, 0x4, 0x00))
#define S_ORI    (SIG(0x13, 0x6, 0x00))
#define S_ANDI   (SIG(0x13, 0x7, 0x00))
#define S_SLLI   (SIG(0x13, 0x1, 0x00))
#define S_SRLI   (SIG(0x13, 0x5, 0x00))
#define S_SRAI   (SIG(0x13, 0x5, 0x20))

#define S_SB (SIG(0x23, 0x0, 0x00))
#define S_SH (SIG(0x23, 0x1, 0x00))
#define S_SW (SIG(0x23, 0x2, 0x00))

#define S_LB  (SIG(0x03, 0x0, 0x00))
#define S_LH  (SIG(0x03, 0x1, 0x00))
#define S_LW  (SIG(0x03, 0x2, 0x00))
#define S_LBU (SIG(0x03, 0x4, 0x00))
#define S_LHU (SIG(0x03, 0x5, 0x00))

#define S_BEQ  (SIG(0x63, 0x9, 0x00))
#define S_BNE  (SIG(0x63, 0x1, 0x00))
#define S_BLT  (SIG(0x63, 0x4, 0x00))
#define S_BGE  (SIG(0x63, 0x5, 0x00))
#define S_BLTU (SIG(0x63, 0x6, 0x00))
#define S_BGEU (SIG(0x63, 0x7, 0x00))

#define S_LUI   (SIG(0x37, 0x0, 0x00))
#define S_AUIPC (SIG(0x17, 0x0, 0x00))
#define S_JAL   (SIG(0x6F, 0x0, 0x00))
#define S_JALR  (SIG(0x67, 0x0, 0x00))


#endif
