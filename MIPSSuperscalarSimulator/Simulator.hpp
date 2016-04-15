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

class Simulator {

public:
    
	Simulator(vector<SimulationInstruction> simulationInstrList);
	FetchStage simuFetch;
	DecodeStage simuDecode;
	ExecuteStage simuExecute;
    MemoryStage simuMemory;
    WriteBackStage simuWriteBack;

    // Register file and main memory will be initialized implicitly (default constructor is provided)
    RegisterFile simuRegFile;
    MainMemory simuMainMemory;
    
    vector<SimulationInstruction> simulationInstructionList;

    int cycleCount;
    int lastStall;
    
    // Order of declaration for member variables is the order of initialization
    SimulationInstruction tempInstr;
    vector<SimulationInstruction> tempInstrList;
    vector<SimulationInstruction> hazardList, tempHazardList;
    bool falsePrediction;
    int instrCount;

    void implement();
    void stepImplement();
    
private:
    vector<int> registerData;

};

#endif /* Simulator_hpp */
