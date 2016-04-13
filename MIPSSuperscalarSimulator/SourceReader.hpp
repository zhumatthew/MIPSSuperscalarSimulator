//
//  SourceReader.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef SourceReader_hpp
#define SourceReader_hpp

#include <stdio.h>
#include <vector>
#include "LabelInstruction.hpp"
#include "Instruction.hpp"

using namespace std;

class SourceReader {
public:
	SourceReader(string filePath);
	string getFilePath();
	void setFilePath(string aFilePath);
	void findLabel();
	void constructInstrList();
	vector<Instruction> getInstructionList();
	vector<Instruction> readInstrList();
	string trim(string str);

private:
    string filePath;
    vector<LabelInstruction> labelInstrList;
    vector<Instruction> instrList;
};

#endif /* SourceReader_hpp */
