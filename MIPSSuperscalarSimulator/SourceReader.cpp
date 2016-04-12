//
//  SourceReader.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <string>
#include "SourceReader.hpp"

using namespace std;

SourceReader::SourceReader(string filePath) {
	this->filePath = filePath;
}

string SourceReader::getFilePath() {
	return this->filePath;
}

void SourceReader::setFilePath(string aFilePath) {
	this->filePath = aFilePath;
}

string SourceReader::trim(string str) {
    if(str.length() == 0) return str;

    int beg = 0, end = str.length() - 1;
    while (str[beg] == ' ') {
        beg++;
    }

    while (str[end] == ' ') {
        end--;
    }

    return str.substr(beg, end - beg + 1);
}

void SourceReader::findLabel() {
	ifstream reader(this->getFilePath());
	string line;
	int lineNumber = 0;
	this->labelInstrList = new vector<LabelInstruction>();

	while (getline(reader, line)) {
		if(line != "") {
			cout << lineNumber << "   " + trim(line);
			cout << "------------------------------------------";
			InstructionParser parser = new InstructionParser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine()[0];

			InstructionType type = new InstructionType();

			if(type.isLabel(strOpcode)) {
				LabelInstruction label = new LabelInstruction(strOpcode,lineNumber);
				this->labelInstrList.push_back(label);
				cout << label.getLabelString() <<  "---------" << label.getLabelAddress();
				cout << this->labelInstrList[this->labelInstrList.size() - 1].getLabelString();
				cout << this->labelInstrList.size();
			}
			lineNumber ++ ;
		}
	}
}

void SourceReader::constructInstrList() {
	ifstream reader(this->getFilePath());
	string line;
	int lineNumber = 0;
	this->instrList = new vector<Instruction>();

	while (getline(reader, line)) {
		if(line != "") {
			cout << lineNumber+"   " + trim(line);
			cout << "------------------------------------------";
			InstructionParser parser = new InstructionParser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine()[0];
			string results[] = parser.getSplitLine();

			InstructionType type = new InstructionType();
			int instrType = type.instrTypeDefine(strOpcode);
			Instruction instr = new Instruction(results,instrType,this->labelInstrList);
			instr.originalString = line;
			cout << "Opcode" << "----->"+ instr.opcode;
			cout << "rs" << "----->"+ instr.rs;
			cout << "rt" << "----->"+ instr.rt;
			cout << "rd" << "----->"+ instr.rd;
			cout << "immediate" + "----->"+ instr.immediate;
			cout << "lowSixDigital" + "----->"+ instr.lowSixDigital;
			cout << "middleFiveDigital" + "----->"+ instr.middleFiveDigital;
			instrList.push_back(instr);
			lineNumber++ ;
		}
	}
}

vector<Instruction> SourceReader::getInstrucionList() {
	return this->instrList;
}

void SourceReader::main() {
    SourceReader sourceReader = new SourceReader("load_delay_1.asm");
    sourceReader.findLabel();
    sourceReader.constructInstrList();
}
