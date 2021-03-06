//
//  Simulator.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Simulator.hpp"
#include <stdio.h>
#include <vector>
#include <algorithm>

#define HAZARD_LIST_SIZE 6
#define OUTPUT_WIDTH 42

using namespace std;

// Classes with default constructors do not need to be explicitly initialized
// Default constructors are implicitly defined unless a constructor is explicitly defined
// Default constructors are constructors that do not require any parameters.
// Default constructors may define default parameters

Simulator::Simulator(vector<SimulatedInstruction> simulatedInstructionList) : simulatedInstructionList(simulatedInstructionList), fetchStage(FetchStage(static_cast<int>(simulatedInstructionList.size()))), committedInstructionCount(0), cycleCount(0), lastStall(0), hazardList(HAZARD_LIST_SIZE, SimulatedInstruction()), tempHazardList(4, SimulatedInstruction()), tempInstrList(2, SimulatedInstruction()) {}

void Simulator::printFinalOutput() {
    cout << string(OUTPUT_WIDTH, '-') << endl;
    cout << "Committed instruction count: " << committedInstructionCount << endl;
    cout << "Total number of cycles: " << cycleCount << endl;
    cout << "CPI: " << static_cast<double>(cycleCount) / static_cast<double>(committedInstructionCount)  << endl;
}

void Simulator::process() {
    // For the pipeline to stop, an "end" is inserted at the end of the benchmark (the pipeline stops when an "end" is detected in the MEM stage). To stay within array borders, three "NOP" are inserted after "end".
    while (memoryStage.currentInstructionList.front().originalString != "end") {
        stepProcess();
    }
}

void Simulator::stepProcess() {
    
    cout << string(OUTPUT_WIDTH, '-') << endl;
    cout << "Clock Cycle #" << ++cycleCount << endl;
    
    // IF -> ID -> EX -> MEM -> WB
    if (lastStall != 2) {
        writeBackStage.currentInstructionList = memoryStage.currentInstructionList;
        memoryStage.currentInstructionList = executeStage.currentInstructionList;
        executeStage.currentInstructionList = decodeStage.currentInstructionList;
        decodeStage.currentInstructionList = fetchStage.currentInstructionList;
    } else {
        
        // At the second cycle since the RAW hazard was detected (lastStall == 2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        writeBackStage.currentInstructionList = memoryStage.currentInstructionList;
        
        // Temporary storage of instructions for forwarding to this cycle's execution stage
        tempInstrList = memoryStage.currentInstructionList;
        
        // Are the memoryStage's instructions wiped? This leads to the process of memoryStage to return immediately without performing any functions. Then, rs and rt of the instructions in the execute stage are forwarded from these "NOP" instructions and tempInstrList is not used.
        
        memoryStage.currentInstructionList[0] = SimulatedInstruction();
        memoryStage.currentInstructionList[1] = SimulatedInstruction();
    }
    
    // If branchMisprediction is set in the execute stage of the previous cycle
    if (branchMisprediction) {
        branchMisprediction = false;
        cout << endl << "Branch misprediction occurred in the previous cycle" << endl;
        cout << "NOP instructions in decode and execute stage" << endl << endl;

        executeStage.currentInstructionList[0] = SimulatedInstruction();
        executeStage.currentInstructionList[1] = SimulatedInstruction();
        decodeStage.currentInstructionList[0] = SimulatedInstruction();
        decodeStage.currentInstructionList[1] = SimulatedInstruction();
        hazardList[2] = tempHazardList[0];
        hazardList[3] = tempHazardList[1];
        hazardList[4] = tempHazardList[2];
        hazardList[5] = tempHazardList[3];
    }
    
    int increment;
    
    cout << "WriteBack:" << writeBackStage.currentInstructionList[0].originalString << endl;
    cout << "WriteBack:" << writeBackStage.currentInstructionList[1].originalString << endl;
    writeBackStage.process(registerFile, increment);
    
    cout << "Memory:" << memoryStage.currentInstructionList[0].originalString << endl;
    cout << "Memory:" << memoryStage.currentInstructionList[1].originalString << endl;
    memoryStage.process(mainMemory, registerFile);
    
    cout << "Execute:" << executeStage.currentInstructionList[0].originalString << endl;
    cout << "Execute:" << executeStage.currentInstructionList[1].originalString << endl;
    executeStage.process(decodeStage, memoryStage, registerFile, lastStall, branchMisprediction);
    
    cout << "Decode:" << decodeStage.currentInstructionList[0].originalString << endl;
    cout << "Decode:" << decodeStage.currentInstructionList[1].originalString << endl;
    decodeStage.process(registerFile, hazardList, lastStall);
    
    fetchStage.process(simulatedInstructionList, lastStall, branchMisprediction, executeStage.getBranchTarget());
    
    cout << "Fetch:" << fetchStage.currentInstructionList[0].originalString << endl;
    cout << "Fetch:" << fetchStage.currentInstructionList[1].originalString << endl;
    committedInstructionCount += increment;

    cout << string(OUTPUT_WIDTH, '-') << endl;
    
    // If a fetched instruction is a branch, then the instructions in the fetch stage and the decode stage need to be stored in tempHazardList.
    
    if (InstructionType::isBranch(fetchStage.currentInstructionList.front().opcode)) {
            tempHazardList[0] = decodeStage.currentInstructionList[0];
            tempHazardList[1] = decodeStage.currentInstructionList[1];
            tempHazardList[2] = fetchStage.currentInstructionList[0];
            tempHazardList[3] = fetchStage.currentInstructionList[1];
        }
        
    switch (lastStall) {
        case 0:
            // Rotate elements of hazardList left by two
            rotate(hazardList.begin(), hazardList.begin() + 2, hazardList.end());
        case 2:
            // Assign the last two elements of hazardList to be the two instructions in the fetch stage
            hazardList[4] = fetchStage.currentInstructionList[0];
            hazardList[5] = fetchStage.currentInstructionList[1];
            break;
        case 1:
            // Rotate elements of hazardList left by two
            rotate(hazardList.begin(), hazardList.begin() + 2, hazardList.end());
            // Assign "NOP" to the last two elements of hazardList
            hazardList[4] = SimulatedInstruction();
            hazardList[5] = SimulatedInstruction();
            break;
    }
    
    for (int i = 0; i < HAZARD_LIST_SIZE; i++) {
        cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
    }
    
    // lastStall indicates the number of cycles that have passed since a load data dependence was detected
    if (lastStall == 2)
        lastStall = 0;
    
    if (lastStall == 1) {
        lastStall = 2;
        decodeStage.loadDataDependence = false;
    }
    if (decodeStage.loadDataDependence) {
        lastStall = 1;
    }
    
}
