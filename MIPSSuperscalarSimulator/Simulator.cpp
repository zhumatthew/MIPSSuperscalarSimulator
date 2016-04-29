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

// if the type of 'decodeStage' is 'DecodeStage' and it has a default constructor, then you don't need to initialize it manually.
// you do not need to define default constructors; they are there implicitly unless you define another constructor

Simulator::Simulator(vector<SimulatedInstruction> simulatedInstructionList) : simulatedInstructionList(simulatedInstructionList), fetchStage(FetchStage(static_cast<int>(simulatedInstructionList.size()))), instructionCount(0), lastStall(0), tempInstr(SimulatedInstruction("nop")), hazardList(HAZARD_LIST_SIZE, tempInstr), tempHazardList(4, tempInstr), tempInstrList(2, tempInstr) {}

//Simulator::Simulator(vector<SimulatedInstruction> simulationInstrList) {
//    simulationInstrList = simulationInstrList;
//    fetchStage = FetchStage((int) simulationInstrList.size());
//    decodeStage = DecodeStage();
//    executeStage = ExecuteStage();
//    memoryStage = MemoryStage();
//    writeBackStage = WriteBackStage();
//    instrCount = 0;
//    lastStall = 0;
//    simuRegFile = RegisterFile();
//    simuMainMemory = MainMemory();
//    tempInstr = SimulatedInstruction("nop");
//    hazardList = vector<SimulatedInstruction>();
//    tempHazardList = vector<SimulatedInstruction>();
//
//    for (int i = 0; i < 6; i++)
//    {
//        hazardList.push_back(tempInstr);
//    }
//    for (int i = 0; i < 4; i++)
//    {
//        tempHazardList.push_back(tempInstr);
//    }
//    tempInstrList = vector<SimulatedInstruction>();
//    for (int i = 0; i < 2; i++)
//    {
//        tempInstrList.push_back(tempInstr);
//    }
//}

void Simulator::process() {
    cycleCount = 0;
    while (memoryStage.currentInstructionList.front().originalString != "end") {
        stepProcess();
    }
    
    cout << "Committed Instruction count: " << instructionCount << endl;
    cout << "Total number of cycles: " << cycleCount << endl;
    cout << "CPI: " << static_cast<double>(cycleCount) / static_cast<double>(instructionCount)  << endl;

}

// Potentially allows the user to press enter to run the entire simulation or press space to run one step at a time

void Simulator::stepProcess() {
    
    cout << "Clock Cycle #" << cycleCount + 1 << endl;
    
    // IF -> ID -> EX -> MEM -> WB
    if (lastStall != 2) {
        writeBackStage.currentInstructionList = memoryStage.currentInstructionList;
        memoryStage.currentInstructionList = executeStage.currentInstructionList;
        executeStage.currentInstructionList = decodeStage.currentInstructionList;
        decodeStage.currentInstructionList = fetchStage.currentInstructionList;
    } else {
        
        // At the second cycle since the RAW hazard was detected (lastStall == 2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        writeBackStage.currentInstructionList = memoryStage.currentInstructionList;
        
        // Temporary storage of instruction for forwarding to this cycle's execution stage
        tempInstrList = memoryStage.currentInstructionList;
        
        // Is the memoryStage's instructions wiped? This leads to the process of memoryStage to return immediately without performing any functions. Then, rs and rt of the instructions in the execute stage are forwarded from these "Empty" instructions and tempInstrList is never used.
        
        memoryStage.currentInstructionList[0] = SimulatedInstruction("Empty");
        memoryStage.currentInstructionList[1] = SimulatedInstruction("Empty");
    }
    
    // If branchMisprediction is set in the execute stage of the previous cycle
    if (branchMisprediction) {
        branchMisprediction = false;
        executeStage.currentInstructionList[0] = SimulatedInstruction("NOP");
        executeStage.currentInstructionList[1] = SimulatedInstruction("NOP");
        decodeStage.currentInstructionList[0] = SimulatedInstruction("NOP");
        decodeStage.currentInstructionList[1] = SimulatedInstruction("NOP");
        hazardList[2] = tempHazardList[0];
        hazardList[3] = tempHazardList[1];
        hazardList[4] = tempHazardList[2];
        hazardList[5] = tempHazardList[3];
    }
    
    int increment;
    
    cout << "WriteBack:" << writeBackStage.currentInstructionList[0].originalString << endl;
    cout << "WriteBack:" << writeBackStage.currentInstructionList[1].originalString << endl;
    writeBackStage.process(registerFile, decodeStage, increment);
    
    cout << "Memory:" << memoryStage.currentInstructionList[0].originalString << endl;
    cout << "Memory:" << memoryStage.currentInstructionList[1].originalString << endl;
    memoryStage.process(mainMemory, registerFile);
    
    cout << "Execute:" << executeStage.currentInstructionList[0].originalString << endl;
    cout << "Execute:" << executeStage.currentInstructionList[1].originalString << endl;
    executeStage.process(decodeStage, memoryStage, registerFile, lastStall, branchMisprediction);
    
    cout << "Decode:" << decodeStage.currentInstructionList[0].originalString << endl;
    cout << "Decode:" << decodeStage.currentInstructionList[1].originalString << endl;
    decodeStage.process(registerFile, hazardList, lastStall);
    
    fetchStage.process(simulatedInstructionList, lastStall, branchMisprediction, executeStage.getSavedProgramCounter());
    
    cout << "Fetch:" << fetchStage.currentInstructionList[0].originalString << endl;
    cout << "Fetch:" << fetchStage.currentInstructionList[1].originalString << endl;
    instructionCount += increment;

    cout << string(OUTPUT_WIDTH, '-') << endl;
    
    // If a fetched instruction is a branch, then the instructions in the fetch stage and the decode stage (which passed through the fetch stage in the last cycle) need to be stored in tempHazardList.
    if (fetchStage.currentInstructionList.front().opcodeString == "BGEZ"
        || fetchStage.currentInstructionList.front().opcodeString == "BLEZ"
        || fetchStage.currentInstructionList.front().opcodeString == "BEQ"
        || fetchStage.currentInstructionList.front().opcodeString == "J") {
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
            // Assign "nop" to the last two elements of hazardList
            hazardList[4] = SimulatedInstruction("nop");
            hazardList[5] = SimulatedInstruction("nop");
            break;
    }
    
    for (int i = 0; i < 6; i++) {
        cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
    }
    
    // lastStall indicates the number of cycles that have passed since a RAW hazard was detected
    if (lastStall == 2)
        lastStall = 0;
    
    if (lastStall == 1) {
        lastStall = 2;
        decodeStage.readAfterWriteHazard = false;
    }
    if (decodeStage.readAfterWriteHazard) {
        lastStall = 1;
    }
    
    cycleCount++;
    cout << string(OUTPUT_WIDTH, '-') << endl;
}

//  void Simulator::main() {
//	SourceReader reader = new SourceReader("D:/Code/AssemblerApp0.99/AssemblerApp/InstructionList.dat");
//	vector<SimulatedInstruction> simuInstrList = vector<SimulatedInstruction>();
//	// vector<Instruction> originInstrList = reader.readInstrList();
//	// System.out.println(originInstrList.size());
//	for(int i =0 ; i < originInstrList.size(); i++){
//	SimuInstr simInstr = new SimuInstr(originInstrList.get(i));
//	simuInstrList.add(simInstr);
//	}
//	SimuInstr endInstr = new SimuInstr("end");
//	simuInstrList.add(endInstr);
//	SimuInstr nopInstr = new SimuInstr("nop");
//	simuInstrList.add(nopInstr);
//	simuInstrList.add(nopInstr);
//	simuInstrList.add(nopInstr);
//	System.out.println(simuInstrList.size());
//  }
