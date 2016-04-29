//
//  InstructionType.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include "InstructionType.hpp"

using namespace std;

// R-type instructions (register)
// I-type instructions (immediate)
// Load/store word instructions/branch comparison     LW r21, 0(r30)       BEQ r1, r0, done
// J-type Instruction   Branch instructions with only immediate     J begina
// branch instructions with rs/immediate      BGEZ r30, endloopb      BGEZ r18, r0, Loop
// label instruction        done ADDI r5, r0, 6

// JAL is jump and link
// SLLI is shift left logical immediate
// SRLI is shift right logical immediate
// R-type shift example is SLL $rd, $rt, shamt:  R[$rd] <- R[$rt] << shamt

// I-type (opcode $rs, $rt, imm):
// bgtz $rs, imm
// blez $rs, imm
// beq $rs, $rt, imm
// lw $rt, imm($rs)
// sw $rt, imm($rs)

// J-type (opcode address):
// j address
// jal address

InstrType InstructionType::instrTypeDefine(string opcode) {
    if (opcode == "add" || opcode == "sub" || opcode == "mult"|| opcode == "div" || opcode == "and" || opcode == "sll"|| opcode == "srl")
        return RType;
    else if (opcode == "addi" || opcode == "andi" || opcode == "ori" || opcode == "xori")
        return IType;
    else if (opcode == "lw" || opcode == "sw"|| opcode == "beq")
        return MBType;
    else if (opcode == "j" || opcode == "jal")
        return JType;
    else if (opcode == "bgtz"|| opcode == "blez")
        return BRIType;
    else
        return Label;
}

bool InstructionType::isLabel(string opcode) {
	InstrType type = instrTypeDefine(opcode);
	if (type == Label)
		return true;
	else
		return false;
}

// case 0 is register-register basic arithmetic
// all but case 0 involve immediates
// case 5 involves labels


// 6-bit operation code
Opcode InstructionType::operationCodeDefine(string str, InstrType instrType) {
    Opcode opcode;
    switch (instrType) {
        case RType:
            if (str == "add") {
                opcode = opcode_add;
            } else if (str == "mult") {
                opcode = opcode_mult;
            } else if (str == "sub") {
                opcode = opcode_sub;
            } else if (str == "div") {
                opcode = opcode_div;
            }
            break;
        case IType:
            if (str == "addi") {
                opcode = opcode_addi;
            }
            break;
        case MBType:
            if (str == "lw") {
                opcode = opcode_lw;
            } else if (str == "sw") {
                opcode = opcode_sw;
            } else if (str == "beq") {
                opcode = opcode_beq;
            }
            break;
        case JType:
            if (str == "j") {
                opcode = opcode_j;
            }
            break;
        case BRIType:
            if (str == "bgtz") {
                opcode = opcode_bgtz;
            } else if (str == "blez") {
                opcode = opcode_blez;
            }
            break;
        default:
            break;
    }
    return opcode;
}

// 6-bit function
Funct InstructionType::functDefine(string str, InstrType instrType) {
	Funct funct = funct_zero;

    if (instrType == RType) {
        if (str == "add") {
            funct = funct_add;
        } else if (str == "mult") {
            funct = funct_mult;
        } else if (str == "sub") {
            funct = funct_sub;
        } else if (str == "div") {
            funct = funct_div;
        } if (str == "sll") {
            funct = funct_sll;
        } if (str == "srl" ) {
            funct = funct_srl;
        }
    }
    return funct;
}
