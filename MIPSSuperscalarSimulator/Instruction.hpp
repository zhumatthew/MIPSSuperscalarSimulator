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
#include "InstructionType.hpp"

class Instruction {
    
public:
    Instruction(vector<string> results, InstrType instructionType, vector<LabelInstruction> labelInstructionList);
    string originalString;
    int funct, shamt, immediate, rd, rt, rs, opcode;
    InstrType instrType;
    string opcodeString;
    void handleInstruction(vector<string> results);
    
private:
    vector<LabelInstruction> labelInstructionList;
    vector<string> removeLabel(vector<string> results);

    
    // Instruction(string str);
};

#endif /* Instruction_hpp */
