#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "riscv.h"

#define F0M 0x7f
#define F1M (0x1f << 7)
#define F2M (0x7 << 12)
#define F3M (0x1f << 15)
#define F4M (0x1f << 20)
#define F5M (0x7f << 25)

#define F0S 0
#define F1S 7
#define F2S 12
#define F3S 15
#define F4S 20
#define F5S 25

typedef enum OPCODE { LUI = 0x37, AUIPC = 0x17,
	       ILType = 0x03, BType = 0x63, JAL = 0x6F,
	       JALR = 0x67, StoreType = 0x23, ILAType = 0x13,
	       RegType = 0x33, ECALL = 0x73
} OPCODE;

typedef enum FUNCT3 { BEQ3=0, BNE3=01, BLT3=04, BGE3=05,
		      BLTU3=0x06, BGEU3=07, LB3=0, LH3=01, LW3=02,
		      LBU3=04, LHU3=05, SB3=0, SH3=01, SW3=02,
		      ADDSUB3=0, SLL3=01, SLT3=02, STLU3=03, XOR3=04,
		      SR3=05, OR3=06, AND3=07, ADDI3=0, ORI3=06, SLTI3=02,
		      XORI3=04, ANDI3=07, SLTIU=03, SLLI3=01, SRI3=05
} FUNCT3;

typedef enum FUNCT7 {
		     ADD7=0, SUB7=0x20, SRA7=0x20, SRL7=0, SRLI7=0x00, SRAI7=0x20
} FUNCT7;


OPCODE get_opcode(int32_t instruction);

FUNCT3 get_funct3(int32_t instruction);

FUNCT7 get_funct7(int32_t instruction);

// get first source register index
uint8_t get_rs1(int32_t instruction);

// get second source register index
uint8_t get_rs2(int32_t instruction);

// get destination register index
uint8_t get_rd(int32_t instruction);

uint8_t get_shamt(int32_t instruction);

int32_t get_imm12_i(int32_t instruction);

int32_t get_imm12_s(int32_t instruction);

int32_t get_imm13(int32_t instruction);

int32_t get_imm20_u(int32_t instruction);

int32_t get_imm21(int32_t instruction);

typedef struct rtype {
  FUNCT7 f7;
  uint8_t rs2;
  uint8_t rs1;
  uint8_t rd;
  FUNCT3 f3;
  OPCODE op;
} rtype;

rtype decode_rtype(int32_t instruction);

typedef struct itype {
  OPCODE op;
  uint8_t rd;
  FUNCT3 f3;
  uint8_t rs1;
  int32_t imm12_i;
} itype;

itype decode_itype(int32_t instruction);

typedef struct isitype {
  OPCODE op;
  uint8_t rd;
  FUNCT3 f3;
  uint8_t rs1;
  uint8_t shamt;
  FUNCT7 f7;
} isitype;

isitype decode_isitype(int32_t instruction);


typedef struct stype {
  OPCODE op;
  FUNCT3 f3;
  uint8_t rs1;
  uint8_t rs2;
  int32_t imm12_s;
} stype;

stype decode_stype(int32_t instruction);

typedef struct sbtype {
  OPCODE op;
  FUNCT3 f3;
  uint8_t rs1;
  uint8_t rs2;
  int32_t imm13;
} sbtype;

sbtype decode_sbtype(int32_t instruction);

typedef struct ujtype {
  OPCODE op;
  uint8_t rd;
  int32_t imm21;
} ujtype;

ujtype decode_ujtype(int32_t instruction);

typedef struct utype {
  OPCODE op;
  uint8_t rd;
  int32_t imm20_u;
} utype;

utype decode_utype(int32_t instruction);

#endif

