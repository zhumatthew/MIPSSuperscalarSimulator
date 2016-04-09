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

Simulator::Simulator(vector<SimulationInstruction*> &simulationInstrList) {
	this->simulationInstrList = simulationInstrList;
	simuFetch = new FetchStage(this->simulationInstrList.size());
	simuDecode = new DecodeStage();
	simuExecute = new ExecuteStage();
	simuMemory = new MemoryStage();
	simuWriteBack = new WriteBackStage();
	this->instrCount = 0;
	this->lastStall = 0;
	simuRegFile = new RegisterFile();
	simuMainMemory = new MainMemory();
	tempInstr = new SimulationInstruction("nop");
	hazardList = vector<SimulationInstruction*>();
	tempHazardList = vector<SimulationInstruction*>();

	for (int i = 0; i < 6; i++)
	{
		hazardList.push_back(tempInstr);
	}
	for (int i = 0; i < 4; i++)
	{
		tempHazardList.push_back(tempInstr);
	}
	tempInstrList = vector<SimulationInstruction*>();
	for (int i = 0; i < 2; i++)
	{
		tempInstrList.push_back(tempInstr);
	}
}

void Simulator::implement() {
	this->cycleCount = 0;
	while (!simuMemory.currentInstructionList[0].originalString == "end")
	{
		wcout << wstring("clockCycle:") << (cycleCount << 1) << endl;
		if (this->lastStall != 2)
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

		if (this->lastStall == 2)
		{
			simuWriteBack.currentInstructionList[0] = simuMemory.currentInstructionList[0];
			simuWriteBack.currentInstructionList[1] = simuMemory.currentInstructionList[1];
			this->tempInstrList[0] = this->simuMemory.currentInstructionList[0];
			this->tempInstrList[1] = this->simuMemory.currentInstructionList[1];
			this->simuMemory.currentInstructionList[0] = new SimulationInstruction("Empty");
			this->simuMemory.currentInstructionList[1] = new SimulationInstruction("Empty");
		}

		if (this->falsePrediction)
		{
			this->falsePrediction = false;
			this->simuExecute.currentInstructionList[0] = new SimulationInstruction("NOP");
			this->simuExecute.currentInstructionList[1] = new SimulationInstruction("NOP");
			this->simuDecode.currentInstructionList[0] = new SimulationInstruction("NOP");
			this->simuDecode.currentInstructionList[1] = new SimulationInstruction("NOP");
			this->hazardList[2] = tempHazardList[0];
			this->hazardList[3] = tempHazardList[1];
			this->hazardList[4] = tempHazardList[2];
			this->hazardList[5] = tempHazardList[3];
		}

		wcout << wstring("Decode:") << simuDecode.currentInstructionList[0].originalString << endl;
		wcout << wstring("Decode:") << simuDecode.currentInstructionList[1].originalString << endl;
		wcout << wstring("Execute:") << simuExecute.currentInstructionList[0].originalString << endl;
		wcout << wstring("Execute:") << simuExecute.currentInstructionList[1].originalString << endl;
		wcout << wstring("Memory:") << simuMemory.currentInstructionList[0].originalString << endl;
		wcout << wstring("Memory:") << simuMemory.currentInstructionList[1].originalString << endl;
		wcout << wstring("WriteBack:") << simuWriteBack.currentInstructionList[0].originalString << endl;
		wcout << wstring("WriteBack:") << simuWriteBack.currentInstructionList[1].originalString << endl;

//        simuWriteBack.implement(simuRegFile, simuDecode, this);
//        simuMemory.implement(simuMainMemory, simuRegFile);
//        simuExecute.implement(null, simuDecode, simuMemory,this);
//        simuDecode.implement(simuMainMemory, simuRegFile, this);
//        simuFetch.implement(simuInstrList, this);



















	}
}
