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
#include "Processor/FetchStage.hpp"
#include "Processor/DecodeStage.hpp"
#include "Processor/ExecuteStage.hpp"
#include "Processor/MemoryStage.hpp"
#include "Processor/WriteBackStage.hpp"
#include "Processor/SimulatedInstruction.hpp"

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
    
    // This flag is set at the execution stage for a conditional branch instruction if a branch is mispredicted. In the cycle following a branch misprediction, instructions in DE/EX are NOP'ed
    bool branchMisprediction;
    
    void printFinalOutput();
    void process();
    void stepProcess();
};

#endif /* Simulator_hpp */
