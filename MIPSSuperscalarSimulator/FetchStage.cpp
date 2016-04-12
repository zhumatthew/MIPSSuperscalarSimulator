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
    
	PC = 0;
	window = vector<SimulationInstruction>();

	for(tempCnt = 0; tempCnt < windowSize; tempCnt++)
		window[tempCnt] = SimulationInstruction("nop");
}

// Program counter may add one or two
void FetchStage::windowMove(vector<SimulationInstruction> simulationInstructionList)
{
	windowTail = 0;
	tempCnt = PC;
    
    // First condition is to ensure window shrinks when approaching the edge of the instruction queue as there are fewer instructions to fetch
    // Second condition is to ensure that the length of the window will not be greater than the window size
    // Instruction size takes into account one "end" and three "nop" at the end of the simulated instruction list
	while (((tempCnt - PC) < (instrSize - PC)) && (windowTail < windowSize)) {
		window[windowTail] = simulationInstructionList[tempCnt];
		if (!window[windowTail].reordered) { // reordered instructions have entered the pipeline; if they enter the window again, they are executed twice
			windowTail++;
		} else { // Avoids a second execution of the instruction after the reordered one
			if (windowTail < 2) {
				PC++;
			}
		}
		tempCnt++;
	}
}

// determines if window[check] can be reordered into window[1] to make a pair with window [0] for simultaneous pipeline entering
bool FetchStage::regNameMatch(int check)
{
	bool tempFlag = false;
	int tempCnti = 0;

	if((window[0].opcodeString == "end") || (window[0].opcodeString == "nop") || (window[0].opcodeString == "NOP"))
		return true;

	while(tempCnti < check) {
		tempCnti++;
		if((window[check].rd == window[0].rd)
			|| (window[check].rd == window[check-tempCnti].rd)
			|| (window[check].rs == window[check-tempCnti].rd)
			|| (window[check].rt == window[check-tempCnti].rd)
			|| (((window[check].rd == window[check-tempCnti].rt) || (window[check].rd == window[check-tempCnti].rs)) && ((check-tempCnti)>2))) {
			tempFlag = true;
			break;
		}
	}
	return tempFlag;
}

// For a pipeline stop, an "end" is inserted at the end of the benchmark (when an "end" is detected in the MEM stage) To stay within array borders, three "nops" are inserted after "end".  "end"/"nop" is not included in reordering, but enters the window/pipeline to stop the pipeline.

void FetchStage::reorder(vector<SimulationInstruction> simulationInstructionList)
{
	if (window[0].opcodeString == "BGEZ" || window[0].opcodeString == "BLEZ" || window[0].opcodeString == "BEQ" || window[0].opcodeString == "J" || window[0].opcodeString == "end" || window[0].opcodeString == "nop" || window[0].opcodeString == "NOP")
		return; // two branch instructions can only enter pipeline with a depth of two (not a single clock cycle) Only a single branch may enter a pipeline for a single cycle. Another condition is false prediction. (pipeline flash cannot be implemented correctly?)
	if ((window[1].rs != window[0].rd) && (window[1].rt != window[0].rd) && (window[1].opcodeString != "end") && (window[1].opcodeString != "nop") && (window[1].opcodeString != "NOP")) {
		pairwise = true; // no data dependence between [0] and [1]; (end cannot enter second depth?)
		return;
	}

	for (tempCnt = 2; tempCnt < windowTail; tempCnt++) {
		if(window[tempCnt].opcodeString == "BGEZ" || window[tempCnt].opcodeString == "BLEZ" || window[tempCnt].opcodeString == "BEQ" || window[tempCnt].opcodeString == "J" || window[tempCnt].opcodeString == "end" || window[tempCnt].opcodeString == "nop" || window[tempCnt].opcodeString == "NOP") // cannot be reordered if it is one of these instructions
			return;
		if (!regNameMatch(tempCnt)) {
			window[tempCnt].reordered = true; // set reordered true for the instruction in the window to prevent it from forwarding
			simulationInstructionList[PC+tempCnt].reordered = true; // set reordered true for the instruction in the instruction queue such that it cannot enter the window on the next cycle
			window.insert(window.begin() + 1, window[tempCnt]); // reorder window[tempcount] to enter pipeline with window[0]
			window.erase(window.begin() + (tempCnt+1));
			pairwise = true;
			return;
		}
	}
}

void FetchStage::clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2)
{
	for (int tempi = cnt1 - 1; tempi >= cnt2; tempi--) {
		simulationInstructionList[tempi].reordered = false;
	}
}

void FetchStage::implement(vector<SimulationInstruction> simulationInstructionList, int lastStall, bool falsePrediction, int savedPC)
{
	pairwise = false;
	lastPC = PC;
	windowMove(simulationInstructionList);
	reorder(simulationInstructionList);

	if (lastStall == 1) {
		return; // no instruction is fetched on a stall
	} else {
		if (pairwise) {
			window[0].loopCount = window[0].instructionLocation + upBranch;
			window[1].loopCount = window[1].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = window[1];
			PC = PC + 2;
			if (window[1].reordered) // The pair of instructions are the result of reordering, so the next sequential address simuInstrList[PC+1] will not enter the pipeline in this cycle
				PC = PC - 1; // simuInstrList[PC+1] will be window[0] the next cycle
		} else { // "nop" is inserted at depth of two and window[0] enters pipeline alone
			window[0].loopCount = window[0].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = SimulationInstruction("NOP");
			PC++;
		}
		clear_reordered(simulationInstructionList, PC, lastPC);

		if (falsePrediction) {
			int updatedPC = savedPC;
			upBranch = upBranch + (PC - updatedPC);
			simulationInstructionList[PC].reordered = false;
			PC = updatedPC;
		}
	}
}
