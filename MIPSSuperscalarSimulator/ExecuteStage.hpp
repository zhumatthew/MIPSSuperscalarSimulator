//
//  ExecuteStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef ExecuteStage_hpp
#define ExecuteStage_hpp

#include <stdio.h>
#include "Stage.hpp"
#include "MemoryStage.hpp"
#include "Simulator.hpp"

using namespace std;

class ExecuteStage : public Stage {

public:
	ExecuteStage();
	void implement(vector<SimulationInstruction> simulationInstructionList, DecodeStage currentDecode, MemoryStage currentMemory, Simulator currentSimulator);
	int getSavedPC();

private:
	int tempPC;
};

#endif /* ExecuteStage_hpp */
