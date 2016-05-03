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
#include "Forward.hpp"
#include "../Assembler/Instruction.hpp"

class SimulatedInstruction {
    
public:
	SimulatedInstruction(); // NOP instruction constructor
    SimulatedInstruction(Instruction instr);
    
    string originalString;
    
    string opcodeString;

    // destination register, target register, source register
    int rd, rt, rs;
    
    // value in destination register, value in target register, value in source register
    int rdValue, rtValue, rsValue;
    
    Opcode opcode;
    Funct funct;
    int immediate;
    int shamt;
    
    // indicates the instruction location in the simulated instruction list
    int instructionLocation;
    
    // this variable is to the sum of the instruction's index in the list and the cumulative address offset incurred from branching.
    int loopCount;
    
    int effectiveAddress;
    
    // The evaluated outcome of a conditional branch.
    bool branchCondition;
    
    // Indicates whether or not this instruction has been reordered.
    bool reordered;
    
    Forward currentForward;

};

#endif /* SimulatedInstruction_hpp */
