//
//  Instruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Instruction.hpp"
#include "InstructionType.hpp"
#include "InstructionParser.hpp"
#include <iostream>

using namespace std;

Instruction::Instruction(vector<string> separatedLine, InstrType type, vector<LabelInstruction> labelInstructionList) : instrType(type), labelInstructionList(labelInstructionList), funct(funct_zero), shamt(0) {

	if (instrType == Label) {
		instrType = InstructionType().instrTypeDefine(separatedLine[1]);
		separatedLine = removeLabel(separatedLine);
	}
    
	opcodeString = separatedLine.front();
	handleInstruction(separatedLine);
}


// opcodeString is assigned the int return value of parseOpcode function??
// name this function parse instruction??
void Instruction::handleInstruction(vector<string> separatedLine) {
    
	InstructionParser parser(separatedLine, labelInstructionList);

	switch (instrType) {
		case RType:
			opcode = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			rt = parser.parseRt(instrType);
			rd = parser.parseRd(instrType);
			shamt = parser.parseShamt(instrType);
			funct = parser.parseFunct(instrType);
			break;
		case IType:
			opcode = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			rd = parser.parseRd(instrType);
			rt = rd;
			immediate = parser.parseImmediate(instrType);
			break;
		case MBType:
			opcode = parser.parseOpcode(instrType);
            rs = parser.parseRs(instrType);
			rt = parser.parseRt(instrType);
			rd = parser.parseRd(instrType);
			immediate = parser.parseImmediate(instrType);
			break;
		case JType:
			opcode = parser.parseOpcode(instrType);
			immediate = parser.parseImmediate(instrType);
			break;
		case BRIType:
			opcode = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			shamt = parser.parseShamt(instrType);
			immediate = parser.parseImmediate(instrType);
			break;
		default:
			cout << "Unknown instruction" << endl;
	}
}

// Removes the label at the start of the separated line
vector<string> Instruction::removeLabel(vector<string> separatedLine) {
    separatedLine.erase(separatedLine.begin());
    return separatedLine;
}
