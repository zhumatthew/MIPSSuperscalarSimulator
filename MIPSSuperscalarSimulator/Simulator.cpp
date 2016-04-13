//
//  Simulator.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <stdio.h>
#include <vector>

#include "Simulator.hpp"

using namespace std;

// if the type of 'simuDecode' is 'DecodeStage' and it has a default constructor, then you don't need to initialize it manually.

Simulator::Simulator(vector<SimulationInstruction> simulationInstrList) : simuFetch(FetchStage((int) simulationInstrList.size())), tempInstr(SimulationInstruction("nop")) {
	simulationInstructionList = simulationInstrList;
	// simuFetch = FetchStage((int) simulationInstrList.size());
//	simuDecode = DecodeStage();
//	simuExecute = ExecuteStage();
//	simuMemory = MemoryStage();
//	simuWriteBack = WriteBackStage();
	instrCount = 0;
	lastStall = 0;
	simuRegFile = RegisterFile();
	simuMainMemory = MainMemory();
	// tempInstr = SimulationInstruction("nop");
	hazardList = vector<SimulationInstruction>();
	tempHazardList = vector<SimulationInstruction>();

	for (int i = 0; i < 6; i++)
	{
		hazardList.push_back(tempInstr);
	}
	for (int i = 0; i < 4; i++)
	{
		tempHazardList.push_back(tempInstr);
	}
	tempInstrList = vector<SimulationInstruction>();
	for (int i = 0; i < 2; i++)
	{
		tempInstrList.push_back(tempInstr);
	}
}

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

void Simulator::implement() {
	cycleCount = 0;
	while (simuMemory.currentInstructionList[0].originalString != "end")
	{
		cout << "clockCycle:" << (cycleCount << 1) << endl;
		if (lastStall != 2)
		{
			simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
			simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
			simuMemory.currentInstructionList[0] = simuExecute.currentInstructionList[0];
			simuMemory.currentInstructionList[1] = simuExecute.currentInstructionList[1];
			simuExecute.currentInstructionList[0] = simuDecode.currentInstructionList[0];
			simuExecute.currentInstructionList[1] = simuDecode.currentInstructionList[1];
			simuDecode.currentInstructionList[0] = simuFetch.currentInstructionList[0];
			simuDecode.currentInstructionList[1] = simuFetch.currentInstructionList[1];
		}

		if (lastStall == 2)
		{
			simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
			simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
			tempInstrList[0] = simuMemory.currentInstructionList[0];
			tempInstrList[1] = simuMemory.currentInstructionList[1];
			simuMemory.currentInstructionList[0] = SimulationInstruction("Empty");
			simuMemory.currentInstructionList[1] = SimulationInstruction("Empty");
		}

		if (falsePrediction)
		{
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

		int increment = simuWriteBack.implement(simuRegFile, simuDecode);
		simuMemory.implement(simuMainMemory, simuRegFile);
		simuExecute.implement(simuDecode, simuMemory, simuRegFile, lastStall, falsePrediction);
		simuDecode.implement(simuMainMemory, simuRegFile, hazardList, lastStall);
		simuFetch.implement(simulationInstructionList, lastStall, falsePrediction, simuExecute.getSavedPC());
        instrCount += increment;

		cout << "Fetch:" << simuFetch.currentInstructionList[0].originalString;
		cout << "Fetch:" << simuFetch.currentInstructionList[1].originalString;
		cout << "Decode:" << simuDecode.currentInstructionList[0].originalString;
		cout << "Decode:" << simuDecode.currentInstructionList[1].originalString;
		cout << "Execute:" << simuExecute.currentInstructionList[0].originalString;
		cout << "Execute:" << simuExecute.currentInstructionList[1].originalString;
		cout << "Memory:" << simuMemory.currentInstructionList[0].originalString;
		cout << "Memory:" << simuMemory.currentInstructionList[1].originalString;
		cout << "WriteBack:" << simuWriteBack.currentInstructionList[0].originalString;
		cout << "WriteBack:" << simuWriteBack.currentInstructionList[1].originalString;
		cout << "-------------------------------------------------";

		if(simuFetch.currentInstructionList[0].opcodeString == "BGEZ"
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
					cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
				}
				break;
			case 1:
				for (int i = 0; i < 4; i++) {
					hazardList[i] = hazardList[i+2];
				}
				hazardList[4] = SimulationInstruction("nop");
				hazardList[5] = SimulationInstruction("nop");

				for (int i = 0; i < 6; i++) {
					cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
				}
				break;
			case 2:
				hazardList[4] = simuFetch.currentInstructionList[0];
				hazardList[5] = simuFetch.currentInstructionList[1]
				                                                 ;
				for (int i = 0; i < 6; i++) {
					cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
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
		cout << "-------------------------------------------------";
	}
	cout << cycleCount;
}

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

	cout << "Decode:" << simuDecode.currentInstructionList[0].originalString;
	cout << "Decode:" << simuDecode.currentInstructionList[1].originalString;
	cout << "Execute:" << simuExecute.currentInstructionList[0].originalString;
	cout << "Execute:" << simuExecute.currentInstructionList[1].originalString;
	cout << "Memory:" << simuMemory.currentInstructionList[0].originalString;
	cout << "Memory:" << simuMemory.currentInstructionList[1].originalString;
	cout << "WriteBack:" << simuWriteBack.currentInstructionList[0].originalString;
	cout << "WriteBack:" << simuWriteBack.currentInstructionList[1].originalString;

    int increment = simuWriteBack.implement(simuRegFile, simuDecode);
	simuMemory.implement(simuMainMemory, simuRegFile);
    simuExecute.implement(simuDecode, simuMemory, simuRegFile, lastStall, falsePrediction);
    simuDecode.implement(simuMainMemory, simuRegFile, hazardList, lastStall);
    simuFetch.implement(simulationInstructionList, lastStall, falsePrediction, simuExecute.getSavedPC());
    instrCount += increment;

	cout << "Fetch:" + simuFetch.currentInstructionList[0].originalString;
	cout << "Fetch:" + simuFetch.currentInstructionList[1].originalString;
	cout << "Decode:" + simuDecode.currentInstructionList[0].originalString;
	cout << "Decode:" + simuDecode.currentInstructionList[1].originalString;
	cout << "Execute:" + simuExecute.currentInstructionList[0].originalString;
	cout << "Execute:" + simuExecute.currentInstructionList[1].originalString;
	cout << "Memory:" + simuMemory.currentInstructionList[0].originalString;
	cout << "Memory:" + simuMemory.currentInstructionList[1].originalString;
	cout << "WriteBack:" + simuWriteBack.currentInstructionList[0].originalString;
	cout << "WriteBack:" + simuWriteBack.currentInstructionList[1].originalString;
	cout << "-------------------------------------------------";

	if(simuFetch.currentInstructionList[0].opcodeString == "BGEZ"
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
			for(int i = 0; i < 4; i++){
				hazardList[i] = hazardList[i+2];
			}
			hazardList[4] = simuFetch.currentInstructionList[0];
			hazardList[5] = simuFetch.currentInstructionList[1];

			for(int i = 0; i < 6; i++){
			cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
			}
			break;
		case 1:
			for(int i = 0; i < 4; i++){
				hazardList[i] = hazardList[i+2];
			}
			hazardList[4] = SimulationInstruction("nop");
			hazardList[5] = SimulationInstruction("nop");

			for(int i = 0; i < 6; i++){
				cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
			}
			break;
		case 2:
			hazardList[4] = simuFetch.currentInstructionList[0];
			hazardList[5] = simuFetch.currentInstructionList[1];
			for(int i = 0; i < 6; i++){
				cout << "hazardList[" << i << "]: " << hazardList[i].originalString;
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
		cout << "-------------------------------------------------";
}

void Simulator::main() {
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
}
