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

// enumeration for instruction types??

// array?
Instruction::Instruction(vector<string> results, InstrType type, vector<LabelInstruction> labelInstrList) : instrType(type), labelInstructionList(labelInstrList) {

	InstructionType instructionType= InstructionType();

	if (instrType == Label) {
		instrType = instructionType.instrTypeDefine(results[1]);
		results = resultDec(results);
	}
	opcodeString = results[0];
	handleInstruction(results);
}


// name this function parse instruction??
void Instruction::handleInstruction(vector<string> results) {
    // this needs to be a vector for input, not an array of strings or something
	InstructionParser parser = InstructionParser(results, labelInstructionList);

	switch (instrType) {
		case RType:
			opcodeString = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			rt = parser.parseRt(instrType);
			rd = parser.parseRd(instrType);
			middleFiveDigital = parser.parseMiddleFiveDigit(instrType);
			lowSixDigital = parser.parseLowSixDigit(instrType);
			break;
		case IType:
			opcode = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			rd = parser.parseRd(instrType);
			rt = rd;
			immediate = parser.parseImmediateNumber(instrType);
			break;
		case MBType:
			opcode = parser.parseOpcode(instrType);
			rt = parser.parseRt(instrType);
			rd = parser.parseRd(instrType);
			rs = parser.parseRs(instrType);
			immediate = parser.parseImmediateNumber(instrType);
			break;
		case JType:
			opcode = parser.parseOpcode(instrType);
			immediate = parser.parseImmediateNumber(instrType);
			break;
		case BRIType:
			opcode = parser.parseOpcode(instrType);
			rs = parser.parseRs(instrType);
			middleFiveDigital = parser.parseMiddleFiveDigit(instrType);
			immediate = parser.parseImmediateNumber(instrType);
			break;
		default:
			cout << "instrType Error" << endl;
	}
}

// call this function removeLabel??
vector<string> Instruction::resultDec(vector<string> results) {
    results.erase(results.begin());
    return results;
}
