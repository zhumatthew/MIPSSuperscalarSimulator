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

    originalString = str;
    opcode = 0;
    rd = 0;
    rs = 0;
    rt = 0;
    rdValue = 0;
    rtValue = 0;
    rsValue = 0;
    immediate = 0;
    originalString = str;
    opcodeString = str;
    currentForward = new Forwarding();
    effectiveAddress = 0;
    branchCondition = false;
    reordered = false;
    instructionLocation = 0;
    loopCount = 0;
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
    originalString = instr.originalString;
    opcodeString = instr.opcodeString;
    currentForward = new Forwarding();
    effectiveAddress = 0;
    branchCondition = false;
    reordered = false;
}
