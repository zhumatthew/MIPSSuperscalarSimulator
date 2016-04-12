//
//  FetchStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "FetchStage.hpp"

// No "this" required unless parameter/local variable name overloading

FetchStage::FetchStage(int instructionLength) : lastPC(0), instrSize(instructionLength), windowSize(8), pairwise(false), windowTail(0), upBranch(0) {
//    super();
	PC = 0;
//	lastPC = 0;
//	instrSize = instructionLength;
//	windowSize = 8;
//	windowTail = 0;
//	pairwise = false;
//	upBranch = 0;
	window = vector<SimulationInstruction>();

	for(tempCnt = 0; tempCnt < windowSize; tempCnt++)
		window[tempCnt] = SimulationInstruction("nop");
}

void FetchStage::windowMove(vector<SimulationInstruction> simulationInstructionList)
{
	windowTail = 0;
	tempCnt = PC;

	while (((tempCnt - PC) < (instrSize - PC)) && (windowTail < windowSize)) {
		window[windowTail] = simulationInstructionList[tempCnt];
		if (!window[windowTail].reordered) {
			windowTail++;
		} else {
			if (windowTail < 2) {
				PC++;
			}
		}
		tempCnt++;
	}
}

bool FetchStage::regNameMatch(int check)
{
	bool tempFlag = false;
	int tempCnti = 0;

	if((window[0].opcodeString == "end") || (window[0].opcodeString == "nop") || (window[0].opcodeString == "NOP"))
		return true;

	while(tempCnti < check)
	{
		tempCnti++;
		if((window[check].rd == window[0].rd)
			|| (window[check].rd == window[check-tempCnti].rd)
			|| (window[check].rs == window[check-tempCnti].rd)
			|| (window[check].rt == window[check-tempCnti].rd)
			|| (((window[check].rd == window[check-tempCnti].rt) || (window[check].rd == window[check-tempCnti].rs)) && ((check-tempCnti)>2)))
		{
			tempFlag = true;
			break;
		}
	}
	return tempFlag;
}

void FetchStage::reorder(vector<SimulationInstruction> simulationInstructionList)
{
	if(window[0].opcodeString == "BGEZ" || window[0].opcodeString == "BLEZ" || window[0].opcodeString == "BEQ" || window[0].opcodeString == "J" || window[0].opcodeString == "end" || window[0].opcodeString == "nop" || window[0].opcodeString == "NOP")
		return;
	if ((window[1].rs != window[0].rd) &&(window[1].rt != window[0].rd) &&(window[1].opcodeString != "end") &&(window[1].opcodeString != "nop") && (window[1].opcodeString != "NOP")) {
		pairwise = true;
		return;
	}

	for (tempCnt=2; tempCnt<windowTail; tempCnt++ )
	{
		if(window[tempCnt].opcodeString == "BGEZ" || window[tempCnt].opcodeString == "BLEZ" || window[tempCnt].opcodeString == "BEQ" || window[tempCnt].opcodeString == "J" || window[tempCnt].opcodeString == "end" || window[tempCnt].opcodeString == "nop" || window[tempCnt].opcodeString == "NOP")
			return;
		if(!regNameMatch(tempCnt)) {
			window[tempCnt].reordered = true;
			simulationInstructionList[PC+tempCnt].reordered = true;
			window.insert(window.begin() + 1, window[tempCnt]);
			window.erase(window.begin() + (tempCnt+1));
			pairwise = true;
			return;
		}
	}
}

void FetchStage::clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2)
{
	int tempi = cnt1 - 1;

	for(;tempi>=cnt2;tempi--)
	{
		simulationInstructionList[tempi].reordered = false;
	}
}

void FetchStage::implement(vector<SimulationInstruction> simulationInstructionList, int lastStall, bool falsePrediction, int savedPC)
{
	pairwise = false;
	this->lastPC = PC;
	windowMove(simulationInstructionList);
	reorder(window, simulationInstructionList);

	if (lastStall == 1) {
		return;
	} else {
		if (pairwise) {
			window[0].loopCount = window[0].instructionLocation + this->upBranch;
			window[1].loopCount = window[1].instructionLocation + this->upBranch;
			this->currentInstructionList[0] = window[0];
			this->currentInstructionList[1] = window[1];
			PC = PC + 2;
			if(window[1].reordered)
				PC = PC - 1;
		} else {
			window[0].loopCount = window[0].instructionLocation + this->upBranch;
			this->currentInstructionList[0] = window[0];
			this->currentInstructionList[1] = SimulationInstruction("NOP");
			PC ++;
		}
		clear_reordered(simulationInstructionList,PC,lastPC);

		if (falsePrediction) {
			int updatedPC = savedPC;
			upBranch = upBranch + (PC - updatedPC);
			simulationInstructionList[PC].reordered = false;
			PC = updatedPC;
		}
	}
}
