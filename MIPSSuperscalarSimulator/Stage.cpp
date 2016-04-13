//
//  Stage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Stage.hpp"

Stage::Stage() {
	currentInstructionList = vector<SimulationInstruction>();
	currentInstructionList.push_back(SimulationInstruction("nop")); // default (initialized) instruction is a NOP
	currentInstructionList.push_back(SimulationInstruction("nop"));
	PC = 0; // Initialize program counter
}
