#include "riscv.h"

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

// get first source register index
uint8_t get_rs1(int32_t instruction);

// get second source register index
uint8_t get_rs2(int32_t instruction);

// get destination register index
uint8_t get_rd(int32_t instruction);

typedef struct rtype {
  FUNCT7 f7;
  int32_t rs2;
  int32_t rs1;
  FUNCT3 f3;
  int32_t rd;
  OPCODE op;
} rtype;

rtype decode_rtype(int32_t instruction);
