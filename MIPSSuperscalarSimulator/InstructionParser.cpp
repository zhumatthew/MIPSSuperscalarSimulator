//
//  InstructionParser.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "InstructionParser.hpp"

// R-type instructions (register?)
// I-type instructions (immediate?)
// Load/store word instructions/branch comparison
// Branch instructions with only immediate
// branch instructions with rs/immediate
// label instruction

//int InstructionType::instrTypeDefine(string strOpcode) {
//    if(strOpcode == "ADD" || strOpcode == "SUB" || strOpcode == "MUL"|| strOpcode == "DIV" || strOpcode == "AND")
//        return instrType = 0;
//    else if(strOpcode == "ADDI" || strOpcode == "ANDI" || strOpcode == "ORI" || strOpcode == "XORI" || strOpcode == "SUBI" || strOpcode == "SLLI"|| strOpcode == "SRLI")
//        return instrType = 1;
//    else if(strOpcode == "LW" || strOpcode == "SW"||strOpcode == "BEQ" || strOpcode == "BEQL")
//        return instrType = 2;
//    else if(strOpcode == "J" || strOpcode == "JAL"|| strOpcode == "B")
//        return instrType = 3;
//    else if(strOpcode == "BGEZ"|| strOpcode == "BLEZ")
//        return instrType = 4;
//    else
//        cout << "It is a label for branch instruction";
//    return instrType = 5;
//}

using namespace std;

InstructionParser::InstructionParser(string line) : line(line) {}

InstructionParser::InstructionParser(vector<string> results, vector<LabelInstruction> labelInstructionList) {
	this->results = results;
	this->labelInstructionList = labelInstructionList;
}

bool starts_with(const string& s1, const string& s2) {
    return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
}

inline bool ends_with(string const & value, string const & ending)
{
    if (ending.size() > value.size()) return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

vector<string> split(const string &text, char sep) {
	vector<string> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != string::npos) {
		string temp = text.substr(start, end - start);
		if (temp != "") tokens.push_back(temp);
		start = end + 1;
	}
	string temp = text.substr(start);
	if (temp != "") tokens.push_back(temp);
	return tokens;
}

void InstructionParser::doSplitLine() {
	this->results = split(this->line, ' ');

	for(int i=0; i < results.size(); i++) {
		if(ends_with(results[i], ",")) {
			results[i] = results[i].substr(0, results[i].length()-1);
		}
	}
}

vector<string> InstructionParser::getSplitLine() {
	return this->results;
}

int InstructionParser::parseOpcode(InstrType instrType) {
	string operation = "";
	operation = this->results[0];

	InstructionType instructionType = InstructionType();
	return instructionType.operationCodeDefine(operation, instrType);
}

int InstructionParser::parseRs(InstrType instrType) {
	int rs = 0;

	switch (instrType) {
		case 0:
			if(starts_with(this->results[2], "r")) {
				string strValue = this->results[2].substr(1 ,this->results[2].length());
				rs = atoi(strValue.c_str());
			}
			break;
		case 1:
			if(starts_with(this->results[2], "r")) {
				string strValue = this->results[2].substr(1,this->results[2].length());
				rs = atoi(strValue.c_str());
			}
		break;
		case 2:
			if(results[0] == "LW") {
				int leftIndex,rightIndex;
				leftIndex = static_cast<int>(results[2].find("("));
				rightIndex = static_cast<int>(results[2].find(")"));
				string str = results[2].substr(leftIndex + 1, rightIndex);
				if (starts_with(str, "r")) {
					string strValue = str.substr(1, str.length());
					rs = atoi(strValue.c_str());
				}
			} else if(results[0] == "SW") {
				int leftIndex,rightIndex;
				leftIndex = static_cast<int>(results[2].find("("));
				rightIndex = static_cast<int>(results[2].find(")"));
				string str = results[2].substr(leftIndex + 1, rightIndex);
				if (starts_with(str, "r")){
					string strValue = str.substr(1,str.length());
					rs = atoi(strValue.c_str());
				}
			} else {
				if(starts_with(results[1], "r"))
					rs = atoi(results[1].substr(1, results[1].length()).c_str());
				}
				break;
		case 3:
			break;
		case 4:
			if(starts_with(this->results[1], "r")){
				string strValue = this->results[1].substr(1,this->results[1].length());
				rs = atoi(strValue.c_str());
			}
			break;
		default:
			rs = 0;
	}
	return rs;
}

// remove extraneous breaks?
int InstructionParser::parseRt(InstrType instrType) {
	int rt = 0;

	switch (instrType) {
		case 0:
			if(starts_with(this->results[3], "r")) {
				string strValue = this->results[3].substr(1, this->results[3].length());
				rt = atoi(strValue.c_str());
			}
			break;
		case 1:
			if(starts_with(this->results[1], "r")){
				string strValue = this->results[1].substr(1, this->results[1].length());
				rt = atoi(strValue.c_str());
			}
			break;
		case 2:
			if (results.front() == "LW") {
				if(starts_with(results[1], "r"))
					rt = atoi(results[1].substr(1, results[1].length()).c_str());
			} else if (results.front() == "SW") {
				if(starts_with(results[1], "r"))
					rt = atoi(results[1].substr(1,results[1].length()).c_str());
			} else {
				if(starts_with(results[2], "r"))
					rt = atoi(results[2].substr(1,results[2].length()).c_str());
			}
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			rt = 0;
	}
	return rt;
}

// remove extraneous breaks?
int InstructionParser::parseRd(InstrType instrType) {
	int rd = 0;
	switch(instrType){
		case 0:
			if(starts_with(this->results[1], "r")) {
				string strValue = this->results[1].substr(1, this->results[1].length());
				rd = atoi(strValue.c_str());
			}
			break;
		case 1:
			if(starts_with(this->results[1], "r")) {
				string strValue = this->results[1].substr(1, this->results[1].length());
				rd = atoi(strValue.c_str());
			}
			break;
		case 2:
			if(results[0] == "LW") {
				if(starts_with(results[1], "r"))
					rd = atoi(results[1].substr(1, results[1].length()).c_str());
			} else if(results[0] == "SW") {

			} else {

			}
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			rd = 0;
	}
	return rd;
}

int InstructionParser::parseLowSixDigit(InstrType instrType) {
	int lowSixDigit = 0;
	InstructionType instructionType = InstructionType();

	switch(instrType) {
		case 0:
			lowSixDigit = instructionType.lowSixDigitDefine(this->results[0], instrType);
			break;
		case 1:
		case 2:
		case 3:
		default:
			lowSixDigit = 0;
	}
	return lowSixDigit;
}

int InstructionParser::parseMiddleFiveDigit(InstrType instrType) {
	int middleFiveDigit = 0;
	InstructionType instructionType = InstructionType();

	switch(instrType){
		case 0:
			middleFiveDigit =instructionType.middleFiveDigitDefine(this->results[0], instrType);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			middleFiveDigit =instructionType.middleFiveDigitDefine(this->results[0], instrType);
			break;
		default:
			middleFiveDigit = 0;
	}
	return middleFiveDigit;
}

// parseImmediateValue?
int InstructionParser::parseImmediateNumber(InstrType instrType) {
	int immediate = 0;

	switch (instrType) {
		case 0:
			break;
		case 1:
			immediate = atoi(this->results[3].c_str());
			break;
		case 2:
			if (results.front() == "LW") {
				int index;
				index = static_cast<int>(results[2].find("("));
				string str = results[2].substr(0, index);
				immediate = atoi(str.c_str());
			} else if (results.front() == "SW") {
				int index;
				index = static_cast<int>(results[2].find("("));
				string str = results[2].substr(0, index);
				immediate = atoi(str.c_str());
			} else {
				for (int index = 0; index < labelInstructionList.size(); index++) {
					if (labelInstructionList[index].getLabelString() == results[3]) {
						immediate = labelInstructionList[index].getLabelAddress();
					}
				}
			}
			break;
		case 3:
			for (int index = 0; index < labelInstructionList.size(); index++) {
				if (labelInstructionList[index].getLabelString() == results[1]) {
					immediate = labelInstructionList[index].getLabelAddress();
				}
			}
			break;
		case 4:
			for (int index = 0; index < labelInstructionList.size(); index++) {
				if (labelInstructionList[index].getLabelString() == results[2]) {
					immediate = labelInstructionList[index].getLabelAddress();
				}
			}
			break;
		default:
            break;
	}
	return immediate;
}
