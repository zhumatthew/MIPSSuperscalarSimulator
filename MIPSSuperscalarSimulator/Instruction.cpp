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

// array?
Instruction::Instruction(vector<string> results, int instructionType, vector<LabelInstruction> labelInstructionList) {
	this->instructionType = instructionType;
	this->labelInstructionList = labelInstructionList;

	InstructionType instrType= InstructionType();

	if(instructionType == 5){
		this->instructionType = instrType.instrTypeDefine(results[1]);
		results = this->resultDec(results);
	}
	this->opcodeString = results[0];
	this->handleInstruction(results);
}

void Instruction::handleInstruction(vector<string> results) {
    // this needs to be a vector for input, not an array of strings or something
	InstructionParser parser = InstructionParser(results, this->labelInstructionList);

	switch(this->instructionType){
		case 0:
			this->opcodeString = parser.parserOptcode(this->instructionType);
			this->rs = parser.parserRs(this->instructionType);
			this->rt = parser.parserRt(this->instructionType);
			this->rd = parser.parserRd(this->instructionType);
			this->middleFiveDigital = parser.parserMiddleFiveDigit(this->instructionType);
			this->lowSixDigital = parser.parserLowSixDigit(this->instructionType);
			break;
		case 1:
			this->opcode = parser.parserOptcode(this->instructionType);
			this->rs = parser.parserRs(this->instructionType);
			this->rd = parser.parserRd(instructionType);
			this->rt = rd;
			this->immediate = parser.parserImmediateNumber(this->instructionType);
			break;
		case 2:
			this->opcode = parser.parserOptcode(this->instructionType);
			this->rt = parser.parserRt(this->instructionType);
			this->rd = parser.parserRd(this->instructionType);
			this->rs = parser.parserRs(this->instructionType);
			this->immediate = parser.parserImmediateNumber(this->instructionType);
			break;
		case 3:
			this->opcode = parser.parserOptcode(this->instructionType);
			this->immediate = parser.parserImmediateNumber(this->instructionType);
			break;
		case 4:
			this->opcode = parser.parserOptcode(this->instructionType);
			this->rs = parser.parserRs(this->instructionType);
			this->middleFiveDigital = parser.parserMiddleFiveDigit(instructionType);
			this->immediate = parser.parserImmediateNumber(instructionType);
			break;
		default:
			cout << "instrType Error";
	}
}

vector<string> Instruction::resultDec(vector<string> results) {
	// vector<string> newResults = string[sizeof(results)/sizeof(results[0])];
    vector<string> newResults;
	for( int i = 1;i < sizeof(results)/sizeof(results[0]); i++ ){
		newResults[i-1] = results[i];
	}
	return newResults;
}
