//
//  Simulator.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Simulator.hpp"
#include <vector>

using namespace std;

//Simulator::Simulator(vector<SimulationInstruction*> &simulationInstrList) {
//	this->simulationInstrList = simulationInstrList;
//	simuFetch = new FetchStage(this->simulationInstrList.size());
//	simuDecode = new DecodeStage();
//	simuExecute = new ExecuteStage();
//	simuMemory = new MemoryStage();
//	simuWriteBack = new WriteBackStage();
//	this->instrCount = 0;
//	this->lastStall = 0;
//	simuRegFile = new RegisterFile();
//	simuMainMemory = new MainMemory();
//	tempInstr = new SimulationInstruction(L"nop");
//	hazardList = vector<SimulationInstruction*>();
//	tempHazardList = vector<SimulationInstruction*>();
//
//	for (int i = 0; i < 6; i++)
//	{
//		hazardList.push_back(tempInstr);
//	}
//	for (int i = 0; i < 4; i++)
//	{
//		tempHazardList.push_back(tempInstr);
//	}
//	tempInstrList = vector<SimulationInstruction*>();
//	for (int i = 0; i < 2; i++)
//	{
//		tempInstrList.push_back(tempInstr);
//	}
// }
