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
    if(str.length() == 0) return str;

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

	while (getline(reader, line)) {
//        if(line != "") {
		if (!line.empty()) {
			cout << lineNumber << "   " + trim(line);
			cout << "------------------------------------------";
			InstructionParser parser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine().front();

			InstructionType type;

			if(type.isLabel(strOpcode)) {
				LabelInstruction label(strOpcode,lineNumber);
				labelInstrList.push_back(label);
				cout << label.getLabelString() <<  "---------" << label.getLabelAddress();
				cout << labelInstrList.back().getLabelString();
				cout << labelInstrList.size();
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

	while (getline(reader, line)) {
//		if(line != "") {
        if (!line.empty()) {
			cout << lineNumber << "   " << trim(line);
			cout << "------------------------------------------";
			InstructionParser parser(line);
			parser.doSplitLine();
			string strOpcode = parser.getSplitLine().front();
			vector<string> results = parser.getSplitLine();

			InstructionType type;
			int instrType = type.instrTypeDefine(strOpcode);
			Instruction instr(results,instrType,labelInstrList);
			instr.originalString = line;
			cout << "Opcode" << "----->" << instr.opcode;
			cout << "rs" << "----->" << instr.rs;
			cout << "rt" << "----->" << instr.rt;
			cout << "rd" << "----->" << instr.rd;
			cout << "immediate" << "----->" << instr.immediate;
			cout << "lowSixDigital" << "----->" << instr.lowSixDigital;
			cout << "middleFiveDigital" << "----->" << instr.middleFiveDigital;
			instrList.push_back(instr);
			lineNumber++;
		}
	}
}

vector<Instruction> SourceReader::getInstructionList() {
	return instrList;
}
