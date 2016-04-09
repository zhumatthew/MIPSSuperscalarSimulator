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

using namespace std;

class SimulationInstruction {
public:
	SimulationInstruction(string str);
    SimulationInstruction(Instruction instr);
    int opcode, rd, rt, rs, rdValue, rtValue, rsValue, immediate;
    string originalString, opcodeString;
    int instructionLocation, loopCount, effectiveAddress;
    bool branchCondition, reordered;
    Forwarding *currentForward;

private:

};

#endif /* SimulationInstruction_hpp */
