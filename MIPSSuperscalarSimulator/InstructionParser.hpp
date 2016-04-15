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
#include "InstructionType.hpp"

using namespace std;

class InstructionParser {
    
public:
	InstructionParser(string aLine);
	InstructionParser(vector<string> results, vector<LabelInstruction> aLabelInstructionList);
	void doSplitLine();
	vector<string> getSplitLine();
	int parseOpcode(InstrType instrType);
	int parseRs(InstrType instrType);
	int parseRt(InstrType instrType);
	int parseRd(InstrType instrType);
	int parseLowSixDigit(InstrType instrType);
	int parseMiddleFiveDigit(InstrType instrType);
	int parseImmediateValue(InstrType instrType);

private:
    string line;
    vector<string> results;
    vector<LabelInstruction> labelInstructionList;

};

#endif /* InstructionParser_hpp */
