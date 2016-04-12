//
//  SimulationInstruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "SimulationInstruction.hpp"
#include "Instruction.hpp"

using namespace std;

SimulationInstruction::SimulationInstruction(string str) : originalString(str), opcode(0), rd(0), rs(0), rt(0), rdValue(0), rtValue(0), rsValue(0), immediate(0), opcodeString(str), currentForward(Forwarding()), effectiveAddress(0), branchCondition(false), reordered(false), instructionLocation(0), loopCount(0) {}

SimulationInstruction::SimulationInstruction(Instruction instr) : rd(instr.rd), rs(instr.rs), rt(instr.rt), rdValue(instr.rdvalue), rsValue(instr.rsvalue), immediate(instr.immediate), originalString(instr.originalString), opcodeString(instr.opcodeString), currentForward(Forwarding()), effectiveAddress(0), branchCondition(false), reordered(false) {}
