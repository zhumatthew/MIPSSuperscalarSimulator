//
//  SimulationInstruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef SimulationInstruction_hpp
#define SimulationInstruction_hpp

#include <stdio.h>
#include <string>
#include "Forwarding.hpp"
#include "Instruction.hpp"

class SimulationInstruction {
    
public:
	SimulationInstruction(string str); // used for nop instruction
    SimulationInstruction(Instruction instr); // used for instructions that are not nop instructions
    // destination register, target register, source register
    int rd, rt, rs;
    // value in destination register, value in target register, value in source register
    int rdValue, rtValue, rsValue;
    int opcode, immediate;
    string originalString, opcodeString;
    int instructionLocation;
    int loopCount;
    int effectiveAddress;
    bool branchCondition, reordered;
    Forwarding currentForward;

};

#endif /* SimulationInstruction_hpp */
