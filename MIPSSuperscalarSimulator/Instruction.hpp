//
//  Instruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "LabelInstruction.hpp"

using namespace std;

class Instruction {
    
public:
    Instruction(string results[], int instructionType, vector<LabelInstruction> labelInstructionList);
    string originalString;
    int lowSixDigital, middleFiveDigital, instructionType, rtvalue, rsvalue, rdvalue, immediate, rd, rt, rs, opcode;
    string opcodeString;
    
private:
    vector<LabelInstruction> labelInstructionList;
    
    // Instruction(string str);
};

#endif /* Instruction_hpp */
