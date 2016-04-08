//
//  AllStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "AllStage.hpp"

AllStage::AllStage() {
	currentInstrList = new vector<SimulationInstruction>();
	currentInstrList.push_back(SimulationInstruction("nop"));
	currentInstrList.push_back(SimulationInstruction("nop"));
	PC = 0;
}
