//
//  FetchStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "FetchStage.hpp"
#include <algorithm>

#define WINDOW_SIZE 8 // maximum number of instructions in the reorder window

// No "this" keyword is required unless there is parameter/local variable name overloading

FetchStage::FetchStage(int instructionListSize) : instructionListSize(instructionListSize), windowTail(0), upBranch(0), window(WINDOW_SIZE, SimulatedInstruction("nop")) {}

// Program counter may add one or two
void FetchStage::windowMove(vector<SimulatedInstruction>& simulatedInstructionList)
{
	windowTail = 0;
	int i = programCounter;
    
    // First condition is to ensure window shrinks when approaching the edge of the instruction queue as there are fewer instructions to fetch
    // Second condition is to ensure that the length of the window will not be greater than the window size
    // Instruction list size takes into account one "end" and three "nop" at the end of the simulated instruction list
	while (((i - programCounter) < (instructionListSize - programCounter)) && (windowTail < WINDOW_SIZE)) {
		window[windowTail] = simulatedInstructionList[i];
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
bool FetchStage::registerNameMatch(int check)
{
    // fuction returns true if a hazard is found
	bool flag = false;

	if ((window[0].opcodeString == "end") || (window[0].opcodeString == "nop") || (window[0].opcodeString == "NOP"))
		return true;
    
//    if (window[check].rd == window[0].rd) flag = true; // WAW hazard (If the loop goes until i <= check), then this instruction is redundant
    
    // Compare window[check] to previous instructions window[0] to window [check-1]
    for (int i = 1; i <= check; i++) {
        
		if ((window[check].rd == window[check-i].rd) // WAW hazard
			|| (window[check].rs == window[check-i].rd) // RAW hazard
			|| (window[check].rt == window[check-i].rd) // RAW hazard (isn't rt the destination for certain instructions?)
			|| (((window[check].rd == window[check-i].rt) || (window[check].rd == window[check-i].rs)) // WAR hazard
                && ((check-i) > 2) // No WAR hazard if earlier instruction (at check-i) reads before the later instruction (at check) updates
                )) {
			flag = true;
			break;
		}
	}
	return flag;
}

// For the pipeline to stop, an "end" is inserted at the end of the benchmark (when an "end" is detected in the MEM stage) To stay within array borders, three "nops" are inserted after "end".  "end"/"nop" is not included in reordering, but enters the window/pipeline to stop the pipeline.

// Returns whether instructions should be executed as a pair

bool FetchStage::reorder(vector<SimulatedInstruction>& simulatedInstructionList)
{
	if (window[0].opcodeString == "BGEZ" || window[0].opcodeString == "BLEZ" || window[0].opcodeString == "BEQ" || window[0].opcodeString == "J" // two branch instructions can only enter pipeline with a depth of two (not a single clock cycle) Only a single branch may enter a pipeline for a single cycle. Another condition is false prediction. (pipeline flash cannot be implemented correctly?) (pipeline flush?)
        || window[0].opcodeString == "end" || window[0].opcodeString == "nop" || window[0].opcodeString == "NOP")
		return false; // window[0] needs to enter the pipeline alone
	if ((window[1].rs != window[0].rd) && (window[1].rt != window[0].rd) && (window[1].opcodeString != "end") && (window[1].opcodeString != "nop") && (window[1].opcodeString != "NOP")) {
		return true; // no data dependence between [0] and [1]; (end cannot enter second depth?)
	}

	for (int i = 2; i < windowTail; i++) {
		if (window[i].opcodeString == "BGEZ" || window[i].opcodeString == "BLEZ" || window[i].opcodeString == "BEQ" || window[i].opcodeString == "J" || window[i].opcodeString == "end" || window[i].opcodeString == "nop" || window[i].opcodeString == "NOP") // cannot be reordered if it is one of these instructions
			return false;
		if (!registerNameMatch(i)) {
			window[i].reordered = true; // set reordered true for the instruction in the window to prevent it from being the source for forwarding
			simulatedInstructionList[programCounter+i].reordered = true; // set reordered true for the instruction in the instruction queue such that it cannot enter the window on the next cycle
            rotate(window.begin() + 1, window.begin() + i, window.begin() + i + 1); // reorder window[tempcount] to enter pipeline with window[0]
			return true;
		}
	}
    return false;
}

void FetchStage::clear_reordered(vector<SimulatedInstruction>& simulatedInstructionList, int cnt1, int cnt2)
{   // reset reordered for all instructions that are executed or leaped over this cycle
	for (int i = cnt1 - 1; i >= cnt2; i--) {
		simulatedInstructionList[i].reordered = false;
	}
}

void FetchStage::process(vector<SimulatedInstruction>& simulatedInstructionList, int lastStall, bool branchMisprediction, int savedPC)
{
	bool pairwise;
	int lastPC = programCounter;
	windowMove(simulatedInstructionList);
	pairwise = reorder(simulatedInstructionList);

	if (lastStall == 1) {
		return; // no instruction is fetched on a stall
	} else {
		if (pairwise) {
			window[0].loopCount = window[0].instructionLocation + upBranch;
			window[1].loopCount = window[1].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = window[1];
			programCounter += 2;
			if (window[1].reordered) // The pair of instructions are the result of reordering, so the next sequential address simulatedInstructionList[PC+1] will not enter the pipeline in this cycle
				programCounter -= 1; // simulatedInstructionList[PC+1] will be window[0] the next cycle
		} else { // "nop" is inserted at second depth and window[0] enters pipeline alone
			window[0].loopCount = window[0].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = SimulatedInstruction("NOP");
			programCounter++;
		}
		clear_reordered(simulatedInstructionList, programCounter, lastPC); // Clear all the instructions that have entered the pipeline this cycle so that when the program counter branches upper address, the instructions can be executed again. If an instruction's reordered flag is set, it cannot enter the instruction window.

        // predict not taken policy, but branch was taken
        // savedPC is the branch target address
		if (branchMisprediction) { // The five stages are processed in reverse order, so the changed PC must not be used by this cycle's fetch stage.  To ensure this, the program counter is only updated after actual issuing is finished using the old program counter.
			int updatedPC = savedPC;
			upBranch = upBranch + (programCounter - updatedPC);
			simulatedInstructionList[programCounter].reordered = false;
			programCounter = updatedPC;
		}
	}
}
