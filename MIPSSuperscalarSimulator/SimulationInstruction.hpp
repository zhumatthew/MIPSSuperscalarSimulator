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
    
    // indicates the instruction location in the simuInstrList
    int instructionLocation;
    
    // this variable is equal to its index in instrList plus total length of upper branch each instruction entered the pipeline and hazard list will has its own loopCnt according to its original index in the instruction queue and its sequence in execution, which will be used to implement correct data forwarding involving reordered instruction
    int loopCount;
    
    int effectiveAddress;
    bool branchCondition;
    
    // Indicates whether this instruction has been reordered. If true, it cannot be the source for data forwarding.
    bool reordered;
    
    Forwarding currentForward;

};

#endif /* SimulationInstruction_hpp */
