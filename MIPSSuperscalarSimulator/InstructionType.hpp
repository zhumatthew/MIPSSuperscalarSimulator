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
    
public:
	InstructionType();
	static int instrTypeDefine(string strOpcode);
	static bool isLabel(string strOpcode);
	static int operatonCodeDefine(string str, int instrType);
	static int lowSixDigitDefine(string str,int instrType);
	static int middleFiveDigitDefine(string str,int instrType);

private:
	static int instrType;
};

#endif /* InstructionType_hpp */
