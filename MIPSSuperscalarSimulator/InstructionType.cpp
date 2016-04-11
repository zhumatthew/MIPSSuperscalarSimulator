//
//  InstructionType.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include "InstructionType.hpp"

using namespace std;

InstructionType::InstructionType() {
    
}

int InstructionType::instrTypeDefine(string strOpcode) {
	if(strOpcode == "ADD" || strOpcode == "SUB" || strOpcode == "MUL"|| strOpcode == "DIV" || strOpcode == "AND")
		return instrType = 0;
	else if(strOpcode == "ADDI" || strOpcode == "ANDI" || strOpcode == "ORI" || strOpcode == "XORI" || strOpcode == "SUBI" || strOpcode == "SLLI"|| strOpcode == "SRLI")
		return instrType = 1;
	else if(strOpcode == "LW" || strOpcode == "SW"||strOpcode == "BEQ" || strOpcode == "BEQL")
		return instrType = 2;
	else if(strOpcode == "J" || strOpcode == "JAL"|| strOpcode == "B")
		return instrType = 3;
	else if(strOpcode == "BGEZ"|| strOpcode == "BLEZ")
		return instrType = 4;
	else
		cout << "It is a label for branch instruction";
	return instrType = 5;
}

bool InstructionType::isLabel(string strOpcode) {
	instrTypeDefine(strOpcode);
	if (this->instrType == 5)
		return true;
	else
		return false;
}

static int InstructionType::operatonCodeDefine(string str, int instrType) {
	int optcode = 0;
	switch(instrType){
		case 0:
			if(str == "ADD") {
				optcode = 0;
			} else if(str == "MUL") {
				optcode =28;
			} else if(str == "SUB") {
				optcode =0;
			} else if(str == "DIV") {
				optcode =0;
			}
		break;
		case 1:
			if(str == "ADDI"){
				optcode = 8;
			}
			break;
		case 2:
			if(str == "LW") {
				optcode = 35;
			} else if(str == "SW") {
				optcode = 43;
			} else if(str == "BEQ") {
				optcode = 4;
			} else if(str == "BEQL") {
				optcode = 20;
			}
			break;
		case 3:
			if(str == "J") {
				optcode = 2;
			}
			break;
		case 4:
			if(str == "BGEZ"){
				optcode = 1;
			} else if(str == "BLEZ") {
				optcode = 6;
			}
			break;
		default:
			optcode = 0;
	}
	return optcode;
}

static int InstructionType::lowSixDigitDefine(string str,int instrType) {
	int low = 0;

	switch(instrType) {
		case 0:
			if(str == "ADD"){
				low = 32;
			} else if(str == "MUL") {
				low = 2;
			} else if(str == "SUB") {
				low = 34;
			} else if(str == "DIV") {
				low = 26;
			}
			break;
		case 1:
		case 2:
		case 3:
			break;
		default:
			low = 0;
	}
	return low;
}

int InstructionType::middleFiveDigitDefine(string str,int instrType) {
	int middle = 0;

	switch(instrType) {
		case 0:
			if(str == "ADD") {
				middle = 0;
			} else if(str == "MUL") {
				middle = 0;
			} else if(str == "SUB") {
				middle = 0;
			} else if(str == "DIV") {
				middle =0;
			}
			break;
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			if(str == "BGEZ"){
				middle = 1;
			} else if(str == "BLEZ") {
				middle = 0;
			}
			break;
		default:
			middle = 0;
	}
	return middle;
}
