//
//  SimulatedInstruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef SimulatedInstruction_hpp
#define SimulatedInstruction_hpp

#include <stdio.h>
#include <string>
#include "Forwarding.hpp"
#include "Instruction.hpp"

class SimulatedInstruction {
    
public:
	SimulatedInstruction(); // used for NOP instruction
    SimulatedInstruction(Instruction instr); // used for instructions that are not nop instructions
    
    string originalString;
    
    string opcodeString;

    // destination register, target register, source register
    int rd, rt, rs;
    
    // value in destination register, value in target register, value in source register
    int rdValue, rtValue, rsValue;
    
    Opcode opcode;
    Funct funct;
    int immediate, shamt;
    
    // indicates the instruction location in the simulated instruction list
    int instructionLocation;
    
    // this variable is equal to the instruction's index in the instruction list plus the total length of upper branch each instruction entered the pipeline and hazard list will have its own loopCount according to its original index in the instruction queue and its sequence in execution, which will be used to implement correct data forwarding involving reordered instructions
    int loopCount;
    
    int effectiveAddress; // Temporarily store the effective address as it is generated in the EX stage and is used in the WB or MEM stage
    
    // The evaluated outcome of a conditional branch.
    bool branchCondition;
    
    // Indicates whether this instruction has been reordered. If true, the instruction cannot be the source for data forwarding.
    bool reordered;
    
    Forwarding currentForward;

};

#endif /* SimulatedInstruction_hpp */
