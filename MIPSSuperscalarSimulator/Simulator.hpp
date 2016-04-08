//
//  Simulator.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <stdio.h>
#include <vector>

#include "MainMemory.hpp"
#include "RegisterFile.hpp"
#include "FetchStage.hpp"
#include "DecodeStage.hpp"
#include "ExecuteStage.hpp"
#include "MemoryStage.hpp"
#include "WriteBackStage.hpp"
#include "SimulationInstruction.hpp"

using namespace std;

class Simulator {

public:
	Simulator(vector<SimulationInstruction*> &simulationInstrList);

	FetchStage simuFetch;
	DecodeStage simuDecode;
	ExecuteStage simuExecute;
    MemoryStage simuMemory;
    WriteBackStage simuWriteBack;

    MainMemory simuMainMemory;
    RegisterFile simuRegFile;
    vector<SimulationInstruction> simulationInstrList;

    int cycleCount;
    int lastStall;
    SimulationInstruction tempInstr;
    vector<SimulationInstruction> tempInstrList;
    bool falsePrediction;
    vector<SimulationInstruction> hazardList;
    vector<SimulationInstruction> tempHazardList;
    int instrCount;

private:
    vector<int> registerData;

};

#endif /* Simulator_hpp */
