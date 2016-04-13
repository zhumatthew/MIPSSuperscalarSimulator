//
//  Instruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef InstructionType_hpp
#define InstructionType_hpp

#include <stdio.h>
#include <string>

//Class InstrType to determine the three type of instruction: Register to
//Register; Register to Immediate; Jump

using namespace std;

class InstructionType {

    // enum InstructionType { register_, immediate,
    
// static members??
    
public:
	int instrTypeDefine(string strOpcode);
	bool isLabel(string strOpcode);
	int operationCodeDefine(string str, int instrType);
	int lowSixDigitDefine(string str,int instrType);
	int middleFiveDigitDefine(string str,int instrType);

private:
	int instrType;
};

#endif /* InstructionType_hpp */
