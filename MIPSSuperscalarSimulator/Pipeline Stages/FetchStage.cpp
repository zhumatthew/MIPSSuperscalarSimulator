//
//  FetchStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "FetchStage.hpp"

// No "this" keyword is required unless there is parameter/local variable name overloading

FetchStage::FetchStage(int instructionLength) : instrSize(instructionLength), windowSize(8), windowTail(0), upBranch(0), window(windowSize, SimulationInstruction("nop")) {}

// Program counter may add one or two
void FetchStage::windowMove(vector<SimulationInstruction> simulationInstructionList)
{
	windowTail = 0;
	int i = programCounter;
    
    // First condition is to ensure window shrinks when approaching the edge of the instruction queue as there are fewer instructions to fetch
    // Second condition is to ensure that the length of the window will not be greater than the window size
    // Instruction size takes into account one "end" and three "nop" at the end of the simulated instruction list
	while (((i - programCounter) < (instrSize - programCounter)) && (windowTail < windowSize)) {
		window[windowTail] = simulationInstructionList[i];
		if (!window[windowTail].reordered) { // reordered instructions have entered the pipeline; if they enter the window again, they are executed twice
			windowTail++;
		} else { // Avoids a second execution of the instruction after the reordered one
			if (windowTail < 2) {
				programCounter++;
			}
		}
		i++;
	}
}

// determines if window[check] can be reordered into window[1] to make a pair with window [0] for simultaneous pipeline entering
bool FetchStage::regNameMatch(int check)
{
	bool flag = false;

	if ((window[0].opcodeString == "end") || (window[0].opcodeString == "nop") || (window[0].opcodeString == "NOP"))
		return true;
    
    for (int i = 0; i < check; i++) {
        
		if ((window[check].rd == window[0].rd)
			|| (window[check].rd == window[check-i].rd)
			|| (window[check].rs == window[check-i].rd)
			|| (window[check].rt == window[check-i].rd)
			|| (((window[check].rd == window[check-i].rt) || (window[check].rd == window[check-i].rs)) && ((check-i) > 2))) {
			flag = true;
			break;
		}
	}
	return flag;
}

// For a pipeline stop, an "end" is inserted at the end of the benchmark (when an "end" is detected in the MEM stage) To stay within array borders, three "nops" are inserted after "end".  "end"/"nop" is not included in reordering, but enters the window/pipeline to stop the pipeline.

// Returns whether instructions should be executed as a pair

bool FetchStage::reorder(vector<SimulationInstruction> simulationInstructionList)
{
	if (window[0].opcodeString == "BGEZ" || window[0].opcodeString == "BLEZ" || window[0].opcodeString == "BEQ" || window[0].opcodeString == "J" || window[0].opcodeString == "end" || window[0].opcodeString == "nop" || window[0].opcodeString == "NOP")
		return false; // two branch instructions can only enter pipeline with a depth of two (not a single clock cycle) Only a single branch may enter a pipeline for a single cycle. Another condition is false prediction. (pipeline flash cannot be implemented correctly?)
	if ((window[1].rs != window[0].rd) && (window[1].rt != window[0].rd) && (window[1].opcodeString != "end") && (window[1].opcodeString != "nop") && (window[1].opcodeString != "NOP")) {
		return true; // no data dependence between [0] and [1]; (end cannot enter second depth?)
	}

	for (int i = 2; i < windowTail; i++) {
		if (window[i].opcodeString == "BGEZ" || window[i].opcodeString == "BLEZ" || window[i].opcodeString == "BEQ" || window[i].opcodeString == "J" || window[i].opcodeString == "end" || window[i].opcodeString == "nop" || window[i].opcodeString == "NOP") // cannot be reordered if it is one of these instructions
			return false;
		if (!regNameMatch(i)) {
			window[i].reordered = true; // set reordered true for the instruction in the window to prevent it from forwarding
			simulationInstructionList[programCounter+i].reordered = true; // set reordered true for the instruction in the instruction queue such that it cannot enter the window on the next cycle
			window.insert(window.begin() + 1, window[i]); // reorder window[tempcount] to enter pipeline with window[0]
			window.erase(window.begin() + (i+1));
			return true;
		}
	}
    return false;
}

void FetchStage::clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2)
{
	for (int i = cnt1 - 1; i >= cnt2; i--) {
		simulationInstructionList[i].reordered = false;
	}
}

void FetchStage::process(vector<SimulationInstruction> simulationInstructionList, int lastStall, bool falsePrediction, int savedPC)
{
	bool pairwise;
	int lastPC = programCounter;
	windowMove(simulationInstructionList);
	pairwise = reorder(simulationInstructionList);

	if (lastStall == 1) {
		return; // no instruction is fetched on a stall
	} else {
		if (pairwise) {
			window[0].loopCount = window[0].instructionLocation + upBranch;
			window[1].loopCount = window[1].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = window[1];
			programCounter = programCounter + 2;
			if (window[1].reordered) // The pair of instructions are the result of reordering, so the next sequential address simuInstrList[PC+1] will not enter the pipeline in this cycle
				programCounter = programCounter - 1; // simuInstrList[PC+1] will be window[0] the next cycle
		} else { // "nop" is inserted at depth of two and window[0] enters pipeline alone
			window[0].loopCount = window[0].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = SimulationInstruction("NOP");
			programCounter++;
		}
		clear_reordered(simulationInstructionList, programCounter, lastPC);

		if (falsePrediction) {
			int updatedPC = savedPC;
			upBranch = upBranch + (programCounter - updatedPC);
			simulationInstructionList[programCounter].reordered = false;
			programCounter = updatedPC;
		}
	}
}
