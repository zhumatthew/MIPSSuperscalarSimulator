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

Instruction::Instruction(vector<string> results, InstrType type, vector<LabelInstruction> labelInstrList) : instrType(type), labelInstructionList(labelInstrList) {

	InstructionType instructionType= InstructionType();

	if (instrType == Label) {
		instrType = instructionType.instrTypeDefine(results[1]);
		results = removeLabel(results);
	}
    
	opcodeString = results.front();
	handleInstruction(results);
}


// opcodeString is assigned the int return value of parseOpcode function??
// name this function parse instruction??
void Instruction::handleInstruction(vector<string> results) {
    
	InstructionParser parser = InstructionParser(results, labelInstructionList);

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
			cout << "instrType Error" << endl;
	}
}

// Remove the label at the start of the results line
vector<string> Instruction::removeLabel(vector<string> results) {
    results.erase(results.begin());
    return results;
}
