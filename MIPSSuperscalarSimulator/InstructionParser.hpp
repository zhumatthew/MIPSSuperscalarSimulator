//
//  InstructionParser.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef InstructionParser_hpp
#define InstructionParser_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "LabelInstruction.hpp"

using namespace std;

class InstructionParser {
    
public:
	InstructionParser(string aLine);
	InstructionParser(vector<string> results, vector<LabelInstruction> aLabelInstructionList);
	void doSplitLine();
	vector<string> getSplitLine();
	int parseOpcode(int instrType);
	int parseRs(int instrType);
	int parseRt(int instrType);
	int parseRd(int instrType);
	int parseLowSixDigit(int instrType);
	int parseMiddleFiveDigit(int instrType);
	int parseImmediateNumber(int instrType);

private:
    string line;
    vector<string> results;
    vector<LabelInstruction> labelInstructionList;

};

#endif /* InstructionParser_hpp */
