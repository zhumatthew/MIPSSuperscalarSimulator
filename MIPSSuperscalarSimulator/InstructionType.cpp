//
//  InstructionType.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include "InstructionType.hpp"

// enum for each opcode
// enum Opcode { ADDI = 8 }
// enum Funct { ADD = 32 }

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

InstrType InstructionType::instrTypeDefine(string strOpcode) {
    if (strOpcode == "ADD" || strOpcode == "SUB" || strOpcode == "MUL"|| strOpcode == "DIV" || strOpcode == "AND")
        return RType;
    else if(strOpcode == "ADDI" || strOpcode == "ANDI" || strOpcode == "ORI" || strOpcode == "XORI" || strOpcode == "SUBI" || strOpcode == "SLLI"|| strOpcode == "SRLI")
        return IType;
    else if(strOpcode == "LW" || strOpcode == "SW"||strOpcode == "BEQ" || strOpcode == "BEQL")
        return MBType;
    else if(strOpcode == "J" || strOpcode == "JAL"|| strOpcode == "B")
        return JType;
    else if(strOpcode == "BGEZ"|| strOpcode == "BLEZ")
        return BRIType;
    else
        cout << "It is a label for branch instruction" << endl;
    return Label;
}

bool InstructionType::isLabel(string strOpcode) {
	InstrType type = instrTypeDefine(strOpcode);
	if (type == Label)
		return true;
	else
		return false;
}

// case 0 is register-register basic arithmetic
// all but case 0 involve immediates
// case 5 involves labels


// 6-bit operation code
int InstructionType::operationCodeDefine(string str, InstrType instrType) {
    int opcode = 0b000000;
    switch (instrType) {
        case RType:
            if (str == "ADD") {
                opcode = 0b000000;
            } else if (str == "MUL") {
                opcode = 0b011100;
            } else if (str == "SUB") {
                opcode = 0b000000;
            } else if (str == "DIV") {
                opcode = 0b000000;
            }
            break;
        case IType:
            if (str == "ADDI") {
                opcode = 0b001000;
            }
            break;
        case MBType:
            if (str == "LW") {
                opcode = 0b100011;
            } else if (str == "SW") {
                opcode = 0b101011;
            } else if (str == "BEQ") {
                opcode = 0b000100;
            } else if (str == "BEQL") {
                opcode = 0b010100;
            }
            break;
        case JType:
            if (str == "J") {
                opcode = 0b000010;
            }
            break;
        case BRIType:
            if (str == "BGEZ") {
                opcode = 0b000001;
            } else if (str == "BLEZ") {
                opcode = 0b000110;
            }
            break;
        default:
            opcode = 0b000000;
    }
    return opcode;
}

// 6-bit function
int InstructionType::functDefine(string str, InstrType instrType) {
	int funct = 0b000000;

	switch (instrType) {
		case RType:
			if (str == "ADD") {
				funct = 0b100000;
			} else if (str == "MUL") {
				funct = 0b000010;
			} else if (str == "SUB") {
				funct = 0b100010;
			} else if (str == "DIV") {
				funct = 0b011010;
			}
			break;
		case IType:
		case MBType:
		case JType:
		default:
            break;
	}
	return funct;
}

// 5-bit shift amount
int InstructionType::shamtDefine(string str, InstrType instrType) {
	int shamt = 0b00000;

	switch (instrType) {
		case RType:
			if (str == "ADD") {
				shamt = 0b00000;
			} else if(str == "MUL") {
				shamt = 0b00000;
			} else if(str == "SUB") {
				shamt = 0b00000;
			} else if(str == "DIV") {
				shamt = 0b00000;
			}
			break;
		case IType:
		case MBType:
		case JType:
			break;
		case BRIType:
			if (str == "BGEZ") {
				shamt = 0b00001;
			} else if (str == "BLEZ") {
				shamt = 0b00000;
			}
			break;
		default:
            break;
	}
	return shamt;
}
