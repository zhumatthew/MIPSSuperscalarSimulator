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
#include <string.h>

#include "Instruction.hpp"

using namespace std;

class SimulationInstruction {
public:
	SimulationInstruction(Instruction instr);
	SimulationInstruction(string str);

    int opcode;
    int rd;
    int rt;
    int rs;
    int rdValue;
    int rtValue;
    int rsValue;
    int immediate;
    string originStr;
    string opcStr;
    int instrLocation;
    int loopCnt;
    Forwarding currentForward;
    int effectiveAddr;
    bool branchCond;
    bool reordered;

private:

};

#endif /* SimulationInstruction_hpp */
