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

#include "Memory/MainMemory.hpp"
#include "Memory/RegisterFile.hpp"
#include "Pipeline Stages/FetchStage.hpp"
#include "Pipeline Stages/DecodeStage.hpp"
#include "Pipeline Stages/ExecuteStage.hpp"
#include "Pipeline Stages/MemoryStage.hpp"
#include "Pipeline Stages/WriteBackStage.hpp"
#include "SimulatedInstruction.hpp"

// Order of declaration for member variables is the order of initialization

class Simulator {

public:
    
    vector<SimulatedInstruction> simulatedInstructionList;
	Simulator(vector<SimulatedInstruction> simulatedInstructionList);
    
	FetchStage fetchStage;
	DecodeStage decodeStage;
	ExecuteStage executeStage;
    MemoryStage memoryStage;
    WriteBackStage writeBackStage;
    
    int committedInstructionCount;
    int cycleCount;
    int lastStall;

    // Register file and main memory will be initialized implicitly (default constructor is provided)
    RegisterFile registerFile;
    MainMemory mainMemory;
    
    vector<SimulatedInstruction> tempInstrList;
    vector<SimulatedInstruction> hazardList, tempHazardList;
    
    // This flag is set at the execution stage of a conditional branch instruction for a mispredicted branch. At the next cycle, the decision of whether or not bubbles will be inserted will be based on this flag.
    bool branchMisprediction;
    
    void printFinalOutput();
    void process();
    void stepProcess();
};

#endif /* Simulator_hpp */
