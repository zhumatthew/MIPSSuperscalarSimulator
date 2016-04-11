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

using namespace std;

class InstructionParser {
    
public:
	InstructionParser(string aLine);
	InstructionParser(vector<string> results, vector<LabelInstruction> aLabelInstructionList);
	void doSplitLine();
	vector<string> getSplitLine();
	int parserOptcode(int instrType);
	int parserRs(int instrType);
	int parserRt(int instrType);
	int parserRd(int instrType);
	int parserLowSixDigit(int instrType);
	int parserMiddleFiveDigit(int instrType);
	int parserImmediateNumber(int instrType);

private:
    string line;
    vector<string> results;
    vector<LabelInstruction> labelInstructionList;

};

#endif /* InstructionParser_hpp */
