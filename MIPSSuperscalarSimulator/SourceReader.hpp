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

using namespace std;

class SourceReader {
public:
	SourceReader(string filePath);
	string getFilePath();
	void setFilePath(string aFilePath);
	void findLabel();
	void constructInstrList();
	vector<Instruction> getInstrucionList();
	vector<Instruction> readInstrList();
	void main();
	string trim(string str);

private:
    string filePath;
    vector<LabelInstruction> labelInstrList;
    vector<Instruction> instrList;
};

#endif /* SourceReader_hpp */
