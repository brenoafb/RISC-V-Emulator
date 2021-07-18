#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

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

typedef enum OPCODE {
		     LUI = 0x37, AUIPC = 0x17,    // upper immediate instructions     (UType)
		     ILType = 0x03,               // Load type                        (IType)
		     BType = 0x63,                // branch                           (SBType)
		     JAL = 0x6F, JALR = 0x67,     // jumps (jal -> uj, jalr -> i)     (UJType)
		     StoreType = 0x23,            // store                            (SType)
		     ILAType = 0x13,              // arithmetic logic with immediate  (IType)
		     RegType = 0x33,              // arithmetic logic with registers  (RType)
		     ECALL = 0x73                 // system call                      (IType)
} OPCODE;

typedef enum FUNCT3 { BEQ3=0, BNE3=01, BLT3=04, BGE3=05, BLTU3=0x06, BGEU3=07,    // branches
		      LB3=0, LH3=01, LW3=02, LBU3=04, LHU3=05,                    // loads
		      SB3=0, SH3=01, SW3=02,                                      // stores
		      ADDSUBMUL3=0, SLL3=01, SLT3=02, STLU3=03,                   // arithmetic logic w/
		      XOR3=04, SR3=05, OR3=06, AND3=07,                           // registers
		      ADDI3=0, ORI3=06, SLTI3=02, XORI3=04, ANDI3=07,             // arithmetic logic w/
		      SLTIU=03, SLLI3=01, SRI3=05                                 // immediate
} FUNCT3;

typedef enum FUNCT7 {
		     ADD7=0, SUB7=0x20, MULDIV7=1, SRA7=0x20, SRL7=0, SRLI7=0x00, SRAI7=0x20
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

typedef struct ifields {
  OPCODE op;
  FUNCT3 f3;
  FUNCT7 f7;

  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;

  uint8_t shamt;
  int32_t imm12_i;
  int32_t imm12_s;
  int32_t imm13;
  int32_t imm20_u;
  int32_t imm21;
} ifields;

ifields decode_instruction(int32_t instruction);

// Generate a signature based on op, f3, and f7 fields.
// Useful for checking which operations the instruction encodes.
uint64_t signature(ifields i);

#endif
