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

using namespace std;

class SimulationInstruction {
public:
	SimulationInstruction(string str);
    int opcode, rd, rt, rs, rdValue, rtValue, rsValue, immediate;
    string originalString, opcodeString;
    int instructionLocation, loopCount, effectiveAddress;
    bool branchCondition, reodered;

private:

};

#endif /* SimulationInstruction_hpp */
