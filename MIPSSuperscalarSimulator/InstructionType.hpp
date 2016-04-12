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

using namespace std;

class InstructionType {
    
public:
	InstructionType();
	int instrTypeDefine(string strOpcode);
	bool isLabel(string strOpcode);
	int operatonCodeDefine(string str, int instrType);
	int lowSixDigitDefine(string str,int instrType);
	int middleFiveDigitDefine(string str,int instrType);

private:
	static int instrType;
};

#endif /* InstructionType_hpp */
