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

//Register to Register; Register to Immediate; Jump

using namespace std;

// Register type, Immediate type, Memory/Branch comparison type, Jump type (only immediate), Branch with Register/Immediate type
enum InstrType {RType, IType, MBType, JType, BRIType, Label};

class InstructionType {
    
// static members??
    
public:
	InstrType instrTypeDefine(string strOpcode);
	bool isLabel(string strOpcode);
	int operationCodeDefine(string str, InstrType instrType);
	int lowSixDigitDefine(string str, InstrType instrType);
	int middleFiveDigitDefine(string str, InstrType instrType);

private:
	InstrType instrType;
};

#endif /* InstructionType_hpp */
