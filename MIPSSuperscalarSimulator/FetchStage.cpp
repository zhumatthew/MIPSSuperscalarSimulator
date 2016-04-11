//
//  FetchStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "FetchStage.hpp"

FetchStage::FetchStage(int instructionLength) {
//    super();
	this->PC = 0;
	this->lastPC = 0;
	this->instrSize = instructionLength;
	this->windowSize = 8;
	this->windowTail = 0;
	this->tempCnt = 0;
	pairwise = false;
	this->upBranch = 0;
	window = vector<SimulationInstruction>();

	for(; tempCnt < windowSize; tempCnt++)
		window[tempCnt] = SimulationInstruction("nop");
}

void FetchStage::windowMove(vector<SimulationInstruction> simulationInstructionList)
{
	windowTail = 0;
	tempCnt = PC;

	while(((tempCnt-PC) < (instrSize-PC)) && (windowTail<windowSize)) {
		window[windowTail] = simulationInstructionList[tempCnt];
		if(!window[windowTail].reordered) {
			windowTail++;
		} else {
			if(windowTail < 2){
				PC++;
			}
		}
		tempCnt++;
	}
}

bool FetchStage::regNameMatch(vector<SimulationInstruction> win, int chk)
{
	bool tempFlag = false;
	int tempCnti = 0;

	if((win[0].opcodeString == "end") || (win[0].opcodeString == "nop") || (win[0].opcodeString == "NOP"))
		return true;

	while(tempCnti < chk)
	{
		tempCnti++;
		if((win[chk].rd == win[0].rd)
			|| (win[chk].rd == win[chk-tempCnti].rd)
			|| (win[chk].rs == win[chk-tempCnti].rd)
			|| (win[chk].rt == win[chk-tempCnti].rd)
			|| (((win[chk].rd == win[chk-tempCnti].rt) || (win[chk].rd == win[chk-tempCnti].rs)) && ((chk-tempCnti)>2)))
		{
			tempFlag = true;
			break;
		}
	}
	return tempFlag;
}

void FetchStage::reorder(vector<SimulationInstruction> win, vector<SimulationInstruction> simulationInstructionList)
{
	if(win[0].opcodeString == "BGEZ" || win[0].opcodeString == "BLEZ" || win[0].opcodeString == "BEQ" || win[0].opcodeString == "J" || win[0].opcodeString == "end" || win[0].opcodeString == "nop" || win[0].opcodeString == "NOP")
		return;
	if ((win[1].rs != win[0].rd) &&(win[1].rt != win[0].rd) &&(!win[1].opcodeString == "end") &&(!win[1].opcodeString == "nop") &&(!win[1].opcodeString == "NOP")) {
		pairwise = true;
		return;
	}

	for( tempCnt=2; tempCnt<windowTail; tempCnt++ )
	{
		if(win[tempCnt].opcodeString == "BGEZ" || win[tempCnt].opcodeString == "BLEZ" || win[tempCnt].opcodeString == "BEQ" || win[tempCnt].opcodeString == "J" || win[tempCnt].opcodeString == "end" || win[tempCnt].opcodeString == "nop" || win[tempCnt].opcodeString == "NOP")
			return;
		if(!regNameMatch(win,tempCnt)) {
			win[tempCnt].reordered = true;
			simulationInstructionList[PC+tempCnt].reordered = true;
			win.insert(win.begin() + 1, win[tempCnt]);
			win.erase(win.begin() + (tempCnt+1));
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

void FetchStage::implement(vector<SimulationInstruction> simulationInstructionList, Simulator currentSimu)
{
	pairwise = false;
	this->lastPC = PC;
	windowMove(simulationInstructionList);
	reorder(window, simulationInstructionList);

	if(currentSimu.lastStall == 1) {
		return;
	} else {
		if(pairwise)
		{
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
			this->currentInstructionList[1] = new SimulationInstruction("NOP");
			PC ++;
		}
		clear_reordered(simulationInstructionList,PC,lastPC);

		if(currentSimu.falsePrediction) {
			int updatedPC = currentSimu.simuExecute.getSavedPC();
			upBranch = upBranch + (PC - updatedPC);
			simulationInstructionList[PC].reordered = false;
			PC = updatedPC;
		}
	}
}
