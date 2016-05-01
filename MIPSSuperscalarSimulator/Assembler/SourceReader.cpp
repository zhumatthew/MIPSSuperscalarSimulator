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

#define OUTPUT_WIDTH 42

using namespace std;

SourceReader::SourceReader(string path) : filePath(path){}

string SourceReader::getFilePath() {
	return filePath;
}

void SourceReader::setFilePath(string filePath) {
	this->filePath = filePath;
}

string SourceReader::trim(string str) {
    if (str.empty()) return str;

    int beg = 0, end = static_cast<int>(str.length()) - 1;
    while (str[beg] == ' ') beg++;
    while (str[end] == ' ') end--;

    return str.substr(beg, end - beg + 1);
}

// Finds addresses of labels for branch instruction targets
void SourceReader::findLabelAddresses() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	labelInstructionList = vector<LabelInstruction>();

	cout << "----------Read the assembly file----------" << endl;

	while (getline(reader, line)) {
		if (!line.empty() && line != "\r") {
			cout << lineNumber << "   " + trim(line) << endl;
			cout << string(OUTPUT_WIDTH, '-') << endl;
			InstructionParser parser(line);
			string strOpcode = parser.splitLine().front();

			InstructionType type;

			if (type.isLabel(strOpcode)) {
				LabelInstruction label(strOpcode, lineNumber);
				labelInstructionList.push_back(label);
			}
			lineNumber++;
		}
	}
}

void SourceReader::constructInstructionList() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	instructionList = vector<Instruction>();

	cout << "-------------Instruction List-------------" << endl;

	while (getline(reader, line)) {
        if (!line.empty() && line != "\r") {
        	cout << string(OUTPUT_WIDTH, '-') << endl;
        	cout << lineNumber++ << "   " << trim(line) << endl;
			InstructionParser parser(line);
			string strOpcode = parser.splitLine().front();
			vector<string> separatedLine = parser.splitLine();

			InstructionType type;
			InstrType instrType = type.instrTypeDefine(strOpcode);
			Instruction instruction(separatedLine, instrType, labelInstructionList);
			instruction.originalString = line;

			cout << "opcode: " << instruction.opcode << endl;
			cout << "rs: " << instruction.rs << endl;
			cout << "rt: " << instruction.rt << endl;
			cout << "rd: " << instruction.rd << endl;
			cout << "immediate: " << instruction.immediate << endl;
			cout << "shamt: " << instruction.shamt << endl;
            cout << "funct: " << instruction.funct << endl;

			instructionList.push_back(instruction);
		}
	}
}

vector<Instruction> SourceReader::getInstructionList() {
	return instructionList;
}
