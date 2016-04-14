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
Instruction::Instruction(vector<string> results, int type, vector<LabelInstruction> labelInstrList) {
	instructionType = type;
	labelInstructionList = labelInstrList;

	InstructionType instrType= InstructionType();

	if (instructionType == 5) {
		instructionType = instrType.instrTypeDefine(results[1]);
		results = resultDec(results);
	}
	opcodeString = results[0];
	handleInstruction(results);
}


// name this function parse instruction??
void Instruction::handleInstruction(vector<string> results) {
    // this needs to be a vector for input, not an array of strings or something
	InstructionParser parser = InstructionParser(results, labelInstructionList);

	switch(instructionType){
		case 0:
			opcodeString = parser.parseOpcode(instructionType);
			rs = parser.parseRs(instructionType);
			rt = parser.parseRt(instructionType);
			rd = parser.parseRd(instructionType);
			middleFiveDigital = parser.parseMiddleFiveDigit(instructionType);
			lowSixDigital = parser.parseLowSixDigit(instructionType);
			break;
		case 1:
			opcode = parser.parseOpcode(instructionType);
			rs = parser.parseRs(instructionType);
			rd = parser.parseRd(instructionType);
			rt = rd;
			immediate = parser.parseImmediateNumber(instructionType);
			break;
		case 2:
			opcode = parser.parseOpcode(instructionType);
			rt = parser.parseRt(instructionType);
			rd = parser.parseRd(instructionType);
			rs = parser.parseRs(instructionType);
			immediate = parser.parseImmediateNumber(instructionType);
			break;
		case 3:
			opcode = parser.parseOpcode(instructionType);
			immediate = parser.parseImmediateNumber(instructionType);
			break;
		case 4:
			opcode = parser.parseOpcode(instructionType);
			rs = parser.parseRs(instructionType);
			middleFiveDigital = parser.parseMiddleFiveDigit(instructionType);
			immediate = parser.parseImmediateNumber(instructionType);
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
