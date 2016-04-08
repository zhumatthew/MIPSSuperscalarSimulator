//
//  Stage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Stage.hpp"

Stage::Stage() {
	currentInstrList = new vector<SimulationInstruction>();
	currentInstrList.push_back(SimulationInstruction("nop"));
	currentInstrList.push_back(SimulationInstruction("nop"));
	PC = 0;
}
