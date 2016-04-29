//
//  SimulatedInstruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "SimulatedInstruction.hpp"
#include "Instruction.hpp"

using namespace std;

SimulatedInstruction::SimulatedInstruction(string str) : originalString(str), opcode(opcode_nop), rd(0), rs(0), rt(0), rdValue(0), rtValue(0), rsValue(0), immediate(0), funct(0), shamt(0), opcodeString(str), currentForward(Forwarding()), effectiveAddress(0), branchCondition(false), reordered(false), instructionLocation(0), loopCount(0) {}

SimulatedInstruction::SimulatedInstruction(Instruction instr) : opcode(instr.opcode), rd(instr.rd), rs(instr.rs), rt(instr.rt), rdValue(0), rtValue(0), rsValue(0), immediate(instr.immediate), funct(instr.funct), shamt(instr.shamt), originalString(instr.originalString), opcodeString(instr.opcodeString), currentForward(Forwarding()), effectiveAddress(0), branchCondition(false), reordered(false) {}
