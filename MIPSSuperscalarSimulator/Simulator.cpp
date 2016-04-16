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

using namespace std;

// if the type of 'simuDecode' is 'DecodeStage' and it has a default constructor, then you don't need to initialize it manually.
// you do not need to define default constructors; they are there implicitly unless you define another constructor

Simulator::Simulator(vector<SimulationInstruction> simulationInstrList) : simulationInstructionList(simulationInstrList), simuFetch(FetchStage((int) simulationInstrList.size())), tempInstr(SimulationInstruction("nop")), instrCount(0), lastStall(0), hazardList(6, tempInstr), tempHazardList(4, tempInstr), tempInstrList(2, tempInstr) {}

//Simulator::Simulator(vector<SimulationInstruction> simulationInstrList) {
//    simulationInstrList = simulationInstrList;
//    simuFetch = FetchStage((int) simulationInstrList.size());
//    simuDecode = DecodeStage();
//    simuExecute = ExecuteStage();
//    simuMemory = MemoryStage();
//    simuWriteBack = WriteBackStage();
//    instrCount = 0;
//    lastStall = 0;
//    simuRegFile = RegisterFile();
//    simuMainMemory = MainMemory();
//    tempInstr = SimulationInstruction("nop");
//    hazardList = vector<SimulationInstruction>();
//    tempHazardList = vector<SimulationInstruction>();
//
//    for (int i = 0; i < 6; i++)
//    {
//        hazardList.push_back(tempInstr);
//    }
//    for (int i = 0; i < 4; i++)
//    {
//        tempHazardList.push_back(tempInstr);
//    }
//    tempInstrList = vector<SimulationInstruction>();
//    for (int i = 0; i < 2; i++)
//    {
//        tempInstrList.push_back(tempInstr);
//    }
//}

void Simulator::process() {
    cycleCount = 0;
    while (simuMemory.currentInstructionList[0].originalString != "end") {
        cout << "clockCycle:" << cycleCount + 1 << endl;
        if (lastStall != 2) {
            simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
            simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
            simuMemory.currentInstructionList[0] = simuExecute.currentInstructionList[0];
            simuMemory.currentInstructionList[1] = simuExecute.currentInstructionList[1];
            simuExecute.currentInstructionList[0] = simuDecode.currentInstructionList[0];
            simuExecute.currentInstructionList[1] = simuDecode.currentInstructionList[1];
            simuDecode.currentInstructionList[0] = simuFetch.currentInstructionList[0];
            simuDecode.currentInstructionList[1] = simuFetch.currentInstructionList[1];
        }
        
        // At the second cycle since the RAW hazard was detected (lastStall==2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        if (lastStall == 2) {
            simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
            simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
            tempInstrList[0] = simuMemory.currentInstructionList[0];
            tempInstrList[1] = simuMemory.currentInstructionList[1];
            simuMemory.currentInstructionList[0] = SimulationInstruction("Empty");
            simuMemory.currentInstructionList[1] = SimulationInstruction("Empty");
        }
        
        if (falsePrediction) {
            falsePrediction = false;
            simuExecute.currentInstructionList[0] = SimulationInstruction("NOP");
            simuExecute.currentInstructionList[1] = SimulationInstruction("NOP");
            simuDecode.currentInstructionList[0] = SimulationInstruction("NOP");
            simuDecode.currentInstructionList[1] = SimulationInstruction("NOP");
            hazardList[2] = tempHazardList[0];
            hazardList[3] = tempHazardList[1];
            hazardList[4] = tempHazardList[2];
            hazardList[5] = tempHazardList[3];
        }
        
        cout << "Decode:" << simuDecode.currentInstructionList[0].originalString << endl;
        cout << "Decode:" << simuDecode.currentInstructionList[1].originalString << endl;
        cout << "Execute:" << simuExecute.currentInstructionList[0].originalString << endl;
        cout << "Execute:" << simuExecute.currentInstructionList[1].originalString << endl;
        cout << "Memory:" << simuMemory.currentInstructionList[0].originalString << endl;
        cout << "Memory:" << simuMemory.currentInstructionList[1].originalString << endl;
        cout << "WriteBack:" << simuWriteBack.currentInstructionList[0].originalString << endl;
        cout << "WriteBack:" << simuWriteBack.currentInstructionList[1].originalString << endl;
        
        int increment = simuWriteBack.process(simuRegFile, simuDecode);
        simuMemory.process(simuMainMemory, simuRegFile);
        simuExecute.process(simuDecode, simuMemory, simuRegFile, lastStall, falsePrediction);
        simuDecode.process(simuMainMemory, simuRegFile, hazardList, lastStall);
        simuFetch.process(simulationInstructionList, lastStall, falsePrediction, simuExecute.getSavedProgramCounter());
        instrCount += increment;
        
        cout << "Fetch:" << simuFetch.currentInstructionList[0].originalString << endl;
        cout << "Fetch:" << simuFetch.currentInstructionList[1].originalString << endl;
        cout << "Decode:" << simuDecode.currentInstructionList[0].originalString << endl;
        cout << "Decode:" << simuDecode.currentInstructionList[1].originalString << endl;
        cout << "Execute:" << simuExecute.currentInstructionList[0].originalString << endl;
        cout << "Execute:" << simuExecute.currentInstructionList[1].originalString << endl;
        cout << "Memory:" << simuMemory.currentInstructionList[0].originalString << endl;
        cout << "Memory:" << simuMemory.currentInstructionList[1].originalString << endl;
        cout << "WriteBack:" << simuWriteBack.currentInstructionList[0].originalString << endl;
        cout << "WriteBack:" << simuWriteBack.currentInstructionList[1].originalString << endl;
        cout << "-------------------------------------------------" << endl;
        
        if (simuFetch.currentInstructionList[0].opcodeString == "BGEZ"
            || simuFetch.currentInstructionList[0].opcodeString == "BLEZ"
            || simuFetch.currentInstructionList[0].opcodeString == "BEQ"
            || simuFetch.currentInstructionList[0].opcodeString == "J") {
            tempHazardList[0] = simuDecode.currentInstructionList[0];
            tempHazardList[1] = simuDecode.currentInstructionList[1];
            tempHazardList[2] = simuFetch.currentInstructionList[0];
            tempHazardList[3] = simuFetch.currentInstructionList[1];
        }
        
        switch (lastStall) {
            case 0:
                for (int i = 0; i < 4; i++){
                    hazardList[i] = hazardList[i+2];
                }
                hazardList[4] = simuFetch.currentInstructionList[0];
                hazardList[5] = simuFetch.currentInstructionList[1];
                
                for (int i = 0; i < 6; i++) {
                    cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
                }
                break;
            case 1:
                for (int i = 0; i < 4; i++) {
                    hazardList[i] = hazardList[i+2];
                }
                hazardList[4] = SimulationInstruction("nop");
                hazardList[5] = SimulationInstruction("nop");
                
                for (int i = 0; i < 6; i++) {
                    cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
                }
                break;
            case 2:
                hazardList[4] = simuFetch.currentInstructionList[0];
                hazardList[5] = simuFetch.currentInstructionList[1];
                
                for (int i = 0; i < 6; i++) {
                    cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
                }
                break;
        }
        
        if (lastStall == 2)
            lastStall = 0;
        
        if (lastStall == 1) {
            lastStall = 2;
            simuDecode.readAfterWriteHazard = false;
        }
        if (simuDecode.readAfterWriteHazard) {
            lastStall = 1;
        }
        cycleCount++;
        cout << "-------------------------------------------------" << endl;
    }
    cout << "Total number of cycles after simulation: " << cycleCount << endl;
}

// Maybe we should allow the user to press enter to run the entire simulation
// or press space to run one step at a time

void Simulator::stepImplement() {
    if (lastStall != 2) {
        simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
        simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
        simuMemory.currentInstructionList[0] = simuExecute.currentInstructionList[0];
        simuMemory.currentInstructionList[1] = simuExecute.currentInstructionList[1];
        simuExecute.currentInstructionList[0] = simuDecode.currentInstructionList[0];
        simuExecute.currentInstructionList[1] = simuDecode.currentInstructionList[1];
        simuDecode.currentInstructionList[0] = simuFetch.currentInstructionList[0];
        simuDecode.currentInstructionList[1] = simuFetch.currentInstructionList[1];
    }
    
    if (lastStall == 2) {
        simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
        simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
        tempInstrList[0] = simuMemory.currentInstructionList[0];
        tempInstrList[1] = simuMemory.currentInstructionList[1];
        simuMemory.currentInstructionList[0] = SimulationInstruction("Empty");
        simuMemory.currentInstructionList[1] = SimulationInstruction("Empty");
    }
    
    if (falsePrediction) {
        falsePrediction = false;
        simuExecute.currentInstructionList[0] =  SimulationInstruction("NOP");
        simuExecute.currentInstructionList[1] = SimulationInstruction("NOP");
        simuDecode.currentInstructionList[0] =  SimulationInstruction("NOP");
        simuDecode.currentInstructionList[1] = SimulationInstruction("NOP");
        hazardList[2] = tempHazardList[0];
        hazardList[3] = tempHazardList[1];
        hazardList[4] = tempHazardList[2];
        hazardList[5] = tempHazardList[3];
    }
    
    cout << "Decode:" << simuDecode.currentInstructionList[0].originalString << endl;
    cout << "Decode:" << simuDecode.currentInstructionList[1].originalString << endl;
    cout << "Execute:" << simuExecute.currentInstructionList[0].originalString << endl;
    cout << "Execute:" << simuExecute.currentInstructionList[1].originalString << endl;
    cout << "Memory:" << simuMemory.currentInstructionList[0].originalString << endl;
    cout << "Memory:" << simuMemory.currentInstructionList[1].originalString << endl;
    cout << "WriteBack:" << simuWriteBack.currentInstructionList[0].originalString << endl;
    cout << "WriteBack:" << simuWriteBack.currentInstructionList[1].originalString << endl;
    
    int increment = simuWriteBack.process(simuRegFile, simuDecode);
    simuMemory.process(simuMainMemory, simuRegFile);
    simuExecute.process(simuDecode, simuMemory, simuRegFile, lastStall, falsePrediction);
    simuDecode.process(simuMainMemory, simuRegFile, hazardList, lastStall);
    simuFetch.process(simulationInstructionList, lastStall, falsePrediction, simuExecute.getSavedProgramCounter());
    instrCount += increment;
    
    cout << "Fetch:" + simuFetch.currentInstructionList[0].originalString << endl;
    cout << "Fetch:" + simuFetch.currentInstructionList[1].originalString << endl;
    cout << "Decode:" + simuDecode.currentInstructionList[0].originalString << endl;
    cout << "Decode:" + simuDecode.currentInstructionList[1].originalString << endl;
    cout << "Execute:" + simuExecute.currentInstructionList[0].originalString << endl;
    cout << "Execute:" + simuExecute.currentInstructionList[1].originalString << endl;
    cout << "Memory:" + simuMemory.currentInstructionList[0].originalString << endl;
    cout << "Memory:" + simuMemory.currentInstructionList[1].originalString << endl;
    cout << "WriteBack:" + simuWriteBack.currentInstructionList[0].originalString << endl;
    cout << "WriteBack:" + simuWriteBack.currentInstructionList[1].originalString << endl;
    cout << "-------------------------------------------------" << endl;
    
    if (simuFetch.currentInstructionList[0].opcodeString == "BGEZ"
       || simuFetch.currentInstructionList[0].opcodeString == "BLEZ"
       || simuFetch.currentInstructionList[0].opcodeString == "BEQ"
       || simuFetch.currentInstructionList[0].opcodeString == "J") {
        tempHazardList[0] =  simuDecode.currentInstructionList[0];
        tempHazardList[1] = simuDecode.currentInstructionList[1];
        tempHazardList[2] = simuFetch.currentInstructionList[0];
        tempHazardList[3] = simuFetch.currentInstructionList[1];
    }
    
    switch (lastStall) {
        case 0:
            for (int i = 0; i < 4; i++) {
                hazardList[i] = hazardList[i+2];
            }
            hazardList[4] = simuFetch.currentInstructionList[0];
            hazardList[5] = simuFetch.currentInstructionList[1];
            
            for (int i = 0; i < 6; i++) {
                cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
            }
            break;
        case 1:
            for (int i = 0; i < 4; i++) {
                hazardList[i] = hazardList[i+2];
            }
            hazardList[4] = SimulationInstruction("nop");
            hazardList[5] = SimulationInstruction("nop");
            
            for (int i = 0; i < 6; i++) {
                cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
            }
            break;
        case 2:
            hazardList[4] = simuFetch.currentInstructionList[0];
            hazardList[5] = simuFetch.currentInstructionList[1];
            for (int i = 0; i < 6; i++) {
                cout << "hazardList[" << i << "]: " << hazardList[i].originalString << endl;
            }
            break;
    }
    
    if(lastStall == 2)
        lastStall = 0;
    
    if(lastStall == 1) {
        lastStall = 2;
        simuDecode.readAfterWriteHazard = false;
    }
    
    if(simuDecode.readAfterWriteHazard) {
        lastStall = 1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "total cycle count : " << cycleCount << endl;
}

//  void Simulator::main() {
//	SourceReader reader = new SourceReader("D:/Code/AssemblerApp0.99/AssemblerApp/InstructionList.dat");
//	vector<SimulationInstruction> simuInstrList = vector<SimulationInstruction>();
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
