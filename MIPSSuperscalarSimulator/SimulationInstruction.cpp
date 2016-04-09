//
//  SimulationInstruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "SimulationInstruction.hpp"
#include "Instruction.hpp"

SimulationInstruction::SimulationInstruction(string str) {

    originStr = str;
    opcode = 0;
    rd = 0;
    rs = 0;
    rt = 0;
    rdValue = 0;
    rtValue = 0;
    rsValue = 0;
    immediate = 0;
    originStr = str;
    opcStr = str;
    currentForward = new Forwarding();
    effectiveAddr = 0;
    branchCond = false;
    reordered = false;
    instrLocation = 0;
    loopCnt = 0;
}

SimulationInstruction::SimulationInstruction(Instruction instr) {

    opcode = instr.opcode;
    rd = instr.rd;
    rs = instr.rs;
    rt = instr.rt;
    rdValue = instr.rdvalue;
    rtValue = instr.rtvalue;
    rsValue = instr.rsvalue;
    immediate = instr.immediate;
    originStr = instr.originalString;
    opcStr = instr.opcodeString;
    currentForward = new Forwarding();
    effectiveAddr = 0;
    branchCond = false;
    reordered = false;
}
