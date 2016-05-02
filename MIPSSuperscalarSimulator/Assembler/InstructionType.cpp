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

// R-type instructions (Register) add $rd, $rs, $rt; sub $rd, $rs, $rt
// I-type instructions (Immediate) bgtz $rs, imm; blez $rs, imm; beq $rs, $rt, imm; lw $rt, imm($rs)
// J-type Instructions (Jump) j address; jal address
// Label instructions

InstrType InstructionType::instrTypeDefine(string opcode) {
    if (opcode == "add" || opcode == "sub" || opcode == "mult"|| opcode == "div" || opcode == "and" || opcode == "sll"|| opcode == "srl")
        return RType;
    else if (opcode == "addi" || opcode == "andi" || opcode == "ori" || opcode == "xori" || opcode == "lw" || opcode == "sw"|| opcode == "beq" || opcode == "bgtz"|| opcode == "blez")
        return IType;
    else if (opcode == "j" || opcode == "jal")
        return JType;
    else
        return Label;
}

bool InstructionType::isLabel(string opcode) {
	if (instrTypeDefine(opcode) == Label)
		return true;
	else
		return false;
}

// Jump instructions are considered to be unconditional branches
bool InstructionType::isBranch(Opcode opcode) {
    return (opcode == opcode_j || opcode == opcode_blez || opcode == opcode_bgtz || opcode == opcode_beq);
}

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
            } else if (str == "lw") {
                opcode = opcode_lw;
            } else if (str == "sw") {
                opcode = opcode_sw;
            } else if (str == "beq") {
                opcode = opcode_beq;
            } else if (str == "bgtz") {
                opcode = opcode_bgtz;
            } else if (str == "blez") {
                opcode = opcode_blez;
            }
            break;
        case JType:
            if (str == "j") {
                opcode = opcode_j;
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
