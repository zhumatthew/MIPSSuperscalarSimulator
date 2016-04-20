//
//  SourceReader.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#include "SourceReader.hpp"
#include "InstructionParser.hpp"
#include "InstructionType.hpp"

using namespace std;

SourceReader::SourceReader(string path) : filePath(path){}

string SourceReader::getFilePath() {
	return filePath;
}

void SourceReader::setFilePath(string filePath) {
	this->filePath = filePath;
}

string SourceReader::trim(string str) {
    if (str.length() == 0) return str;

    int beg = 0, end = static_cast<int>(str.length()) - 1;
    while (str[beg] == ' ') beg++;
    while (str[end] == ' ') end--;

    return str.substr(beg, end - beg + 1);
}

void SourceReader::findLabel() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	labelInstrList = vector<LabelInstruction>();

	cout << "------------Read the assembly File------------" << endl;

	while (getline(reader, line)) {
		if (!line.empty()) {
			cout << lineNumber << "   " + trim(line) << endl;
			cout << "------------------------------------------" << endl;
			InstructionParser parser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine().front();

			InstructionType type;

			if (type.isLabel(strOpcode)) {
				LabelInstruction label(strOpcode,lineNumber);
				labelInstrList.push_back(label);
				cout << label.getLabelString() <<  "---------" << label.getLabelAddress() << endl;
				cout << labelInstrList.back().getLabelString() << endl;
				cout << labelInstrList.size() << endl;
			}
			lineNumber++;
		}
	}
}

void SourceReader::constructInstrList() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	instrList = vector<Instruction>();

	cout << "--------Printing out each component-------" << endl;

	while (getline(reader, line)) {
        if (!line.empty()) {
        	cout << "------------------------------------------" << endl;
        	cout << lineNumber << "   " << trim(line) << endl;
			InstructionParser parser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine().front();
			vector<string> results = parser.getSplitLine();

			InstructionType type;
			InstrType instrType = type.instrTypeDefine(strOpcode);
			Instruction instr(results, instrType, labelInstrList);
			instr.originalString = line;

			cout << "opcode: " << instr.opcode << endl;
			cout << "rs: " << instr.rs << endl;
			cout << "rt: " << instr.rt << endl;
			cout << "rd: " << instr.rd << endl;
			cout << "immediate: " << instr.immediate << endl;
			cout << "funct: " << instr.funct << endl;
			cout << "shamt: " << instr.shamt << endl;
			instrList.push_back(instr);
			lineNumber++;
		}
	}
}

vector<Instruction> SourceReader::getInstructionList() {
	return instrList;
}
