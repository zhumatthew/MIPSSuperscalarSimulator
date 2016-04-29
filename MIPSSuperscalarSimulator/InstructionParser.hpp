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
	InstructionParser(string line);
	InstructionParser(vector<string> results, vector<LabelInstruction> labelInstructionList);
	vector<string> splitLine();
	int parseOpcode(InstrType instrType);
	int parseRs(InstrType instrType);
	int parseRt(InstrType instrType);
	int parseRd(InstrType instrType);
	int parseFunct(InstrType instrType);
	int parseShamt(InstrType instrType);
	int parseImmediate(InstrType instrType);

private:
    string line;
    vector<string> separatedLine; // line split by the delimiter ' ' space character
    vector<LabelInstruction> labelInstructionList;

};

#endif /* InstructionParser_hpp */
