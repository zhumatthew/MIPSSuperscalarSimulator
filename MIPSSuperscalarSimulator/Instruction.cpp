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
Instruction::Instruction(vector<string> results, int instructionType, vector<LabelInstruction> labelInstructionList) {
	instructionType = instructionType;
	labelInstructionList = labelInstructionList;

	InstructionType instrType= InstructionType();

	if (instructionType == 5) {
		instructionType = instrType.instrTypeDefine(results[1]);
		results = resultDec(results);
	}
	opcodeString = results[0];
	handleInstruction(results);
}

void Instruction::handleInstruction(vector<string> results) {
    // this needs to be a vector for input, not an array of strings or something
	InstructionParser parser = InstructionParser(results, labelInstructionList);

	switch(instructionType){
		case 0:
			opcodeString = parser.parserOptcode(instructionType);
			rs = parser.parserRs(instructionType);
			rt = parser.parserRt(instructionType);
			rd = parser.parserRd(instructionType);
			middleFiveDigital = parser.parserMiddleFiveDigit(instructionType);
			lowSixDigital = parser.parserLowSixDigit(instructionType);
			break;
		case 1:
			opcode = parser.parserOptcode(instructionType);
			rs = parser.parserRs(instructionType);
			rd = parser.parserRd(instructionType);
			rt = rd;
			immediate = parser.parserImmediateNumber(instructionType);
			break;
		case 2:
			opcode = parser.parserOptcode(instructionType);
			rt = parser.parserRt(instructionType);
			rd = parser.parserRd(instructionType);
			rs = parser.parserRs(instructionType);
			immediate = parser.parserImmediateNumber(instructionType);
			break;
		case 3:
			opcode = parser.parserOptcode(instructionType);
			immediate = parser.parserImmediateNumber(instructionType);
			break;
		case 4:
			opcode = parser.parserOptcode(instructionType);
			rs = parser.parserRs(instructionType);
			middleFiveDigital = parser.parserMiddleFiveDigit(instructionType);
			immediate = parser.parserImmediateNumber(instructionType);
			break;
		default:
			cout << "instrType Error";
	}
}

// call this function removeLabel??
vector<string> Instruction::resultDec(vector<string> results) {
    results.erase(results.begin());
    return results;
}
