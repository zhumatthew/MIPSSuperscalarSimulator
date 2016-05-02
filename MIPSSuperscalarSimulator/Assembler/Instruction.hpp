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
    Instruction(vector<string> separatedLine, InstrType instructionType, vector<LabelInstruction> labelInstructionList);
    string originalString;
    
    Opcode opcode;
    Funct funct;
    
    int rs, rt, rd;
    int immediate;
    
    // 5-bit shift amount
    int shamt;
    
    InstrType instrType;
    string opcodeString;
    void handleInstruction(vector<string> separatedLine);
    
private:
    vector<LabelInstruction> labelInstructionList;
    vector<string> removeLabel(vector<string> separatedLine);
    
};

#endif /* Instruction_hpp */
