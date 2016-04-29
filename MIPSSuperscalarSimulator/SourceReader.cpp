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

void SourceReader::findLabel() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	labelInstructionList = vector<LabelInstruction>();

    //
	cout << "----------Read the assembly File----------" << endl;

	while (getline(reader, line)) {
		if (!line.empty()) {
			cout << lineNumber << "   " + trim(line) << endl;
            // MATLAB like formatting? printf?
            // String constructor to generate dashes?
			cout << string(OUTPUT_WIDTH, '-') << endl;
			InstructionParser parser(line);
			string strOpcode = parser.splitLine().front();

			InstructionType type;

			if (type.isLabel(strOpcode)) {
				LabelInstruction label(strOpcode,lineNumber);
				labelInstructionList.push_back(label);
			}
			lineNumber++;
		}
	}
}

void SourceReader::constructInstrList() {
	ifstream reader(getFilePath().c_str());
	string line;
	int lineNumber = 0;
	instructionList = vector<Instruction>();

	cout << "-------Printing out each instruction------" << endl;

	while (getline(reader, line)) {
        if (!line.empty()) {
        	cout << string(OUTPUT_WIDTH, '-') << endl;
//            printf("Width trick: %*d \n", 5, 10);
//            printf("%d %*s \n", lineNumber, 3, trim(line).c_str());
        	cout << lineNumber << "   " << trim(line) << endl;
			InstructionParser parser(line);
			string strOpcode = parser.splitLine().front();
			vector<string> results = parser.splitLine();

			InstructionType type;
			InstrType instrType = type.instrTypeDefine(strOpcode);
			Instruction instr(results, instrType, labelInstructionList);
			instr.originalString = line;

			cout << "opcode: " << instr.opcode << endl;
			cout << "rs: " << instr.rs << endl;
			cout << "rt: " << instr.rt << endl;
			cout << "rd: " << instr.rd << endl;
			cout << "immediate: " << instr.immediate << endl;
			cout << "shamt: " << instr.shamt << endl;
            cout << "funct: " << instr.funct << endl;

			instructionList.push_back(instr);
			lineNumber++;
		}
	}
}

vector<Instruction> SourceReader::getInstructionList() {
	return instructionList;
}
