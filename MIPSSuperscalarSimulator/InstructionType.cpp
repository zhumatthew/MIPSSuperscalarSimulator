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

InstrType InstructionType::instrTypeDefine(string strOpcode) {
    if(strOpcode == "ADD" || strOpcode == "SUB" || strOpcode == "MUL"|| strOpcode == "DIV" || strOpcode == "AND")
        return instrType = RType;
    else if(strOpcode == "ADDI" || strOpcode == "ANDI" || strOpcode == "ORI" || strOpcode == "XORI" || strOpcode == "SUBI" || strOpcode == "SLLI"|| strOpcode == "SRLI")
        return instrType = IType;
    else if(strOpcode == "LW" || strOpcode == "SW"||strOpcode == "BEQ" || strOpcode == "BEQL")
        return instrType = MBType;
    else if(strOpcode == "J" || strOpcode == "JAL"|| strOpcode == "B")
        return instrType = JType;
    else if(strOpcode == "BGEZ"|| strOpcode == "BLEZ")
        return instrType = BRIType;
    else
        cout << "It is a label for branch instruction" << endl;
    return instrType = Label;
}

bool InstructionType::isLabel(string strOpcode) {
	instrTypeDefine(strOpcode);
	if (instrType == Label)
		return true;
	else
		return false;
}

// case 0 is register-register basic arithmetic
// all but case 0 involve immediates
// case 5 involves labels


// binary literals?
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
                opcode = 2;
            }
            break;
        case BRIType:
            if (str == "BGEZ") {
                opcode = 1;
            } else if (str == "BLEZ") {
                opcode = 6;
            }
            break;
        default:
            opcode = 0;
    }
    return opcode;
}

int InstructionType::functDefine(string str, InstrType instrType) {
	int funct = 0;

	switch (instrType) {
		case RType:
			if (str == "ADD") {
				funct = 32;
			} else if (str == "MUL") {
				funct = 2;
			} else if (str == "SUB") {
				funct = 34;
			} else if (str == "DIV") {
				funct = 26;
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

int InstructionType::middleFiveDigitDefine(string str, InstrType instrType) {
	int middle = 0;

	switch (instrType) {
		case RType:
			if (str == "ADD") {
				middle = 0;
			} else if(str == "MUL") {
				middle = 0;
			} else if(str == "SUB") {
				middle = 0;
			} else if(str == "DIV") {
				middle = 0;
			}
			break;
		case IType:
		case MBType:
		case JType:
			break;
		case BRIType:
			if (str == "BGEZ") {
				middle = 1;
			} else if (str == "BLEZ") {
				middle = 0;
			}
			break;
		default:
			middle = 0;
	}
	return middle;
}
