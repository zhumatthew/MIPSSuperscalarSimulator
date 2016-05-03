//
//  Instruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef InstructionType_hpp
#define InstructionType_hpp

#include <stdio.h>
#include <string>

using namespace std;

enum InstrType {RType, IType, JType, Label};
enum Opcode { opcode_rtype = 0, opcode_nop = 0, opcode_add = 0, opcode_sub = 0, opcode_mult = 0, opcode_div = 0, opcode_and = 0, opcode_addi = 8, opcode_andi = 12, opcode_ori = 13, opcode_xori = 14, opcode_sll = 0, opcode_srl = 0, opcode_lw = 35, opcode_sw = 43, opcode_beq = 4, opcode_j = 2, opcode_jal = 3, opcode_bgtz = 7, opcode_blez = 6 };
enum Funct { funct_zero = 0, funct_add = 32, funct_mult = 24, funct_sub = 34, funct_div = 26, funct_sll = 0, funct_srl = 2 };


class InstructionType {
        
public:
	static InstrType instrTypeDefine(string strOpcode);
	static Opcode operationCodeDefine(string str, InstrType instrType);
	static Funct functDefine(string str, InstrType instrType);
    static bool isBranch(Opcode opcode) {return (opcode == opcode_j || opcode == opcode_blez || opcode == opcode_bgtz || opcode == opcode_beq); } // Jump instructions are considered to be unconditional branches
    static bool isLabel(string opcode) { return instrTypeDefine(opcode) == Label; }
    
};

#endif /* InstructionType_hpp */
