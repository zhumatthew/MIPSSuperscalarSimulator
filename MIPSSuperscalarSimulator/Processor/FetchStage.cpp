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

FetchStage::FetchStage(int instructionListSize) : instructionListSize(instructionListSize), windowTail(0), upBranch(0), window(WINDOW_SIZE, SimulatedInstruction()) {}

// Program counter may add one or two
void FetchStage::windowMove(vector<SimulatedInstruction>& simulatedInstructionList)
{
	windowTail = 0;
	int i = programCounter;
    
    // First condition is to ensure that the the difference between the program counter and 'i' (the index of the remaining instructions between the PC and the end of the list) does not exceed the difference between the program counter and the total instruction list's size (the number of remaining instructions between the PC and the end of the list)
    // Second condition is to ensure that the length of the window will not be greater than the window size
    // Instruction list size takes into account one "end" and three "NOP" at the end of the simulated instruction list
	while (((i - programCounter) < (instructionListSize - programCounter)) && (windowTail < WINDOW_SIZE)) {
		window[windowTail] = simulatedInstructionList[i];
		if (!window[windowTail].reordered) { // reordered instructions have entered the pipeline; if they enter the window again, they are executed twice
			windowTail++;
		} else { // Avoids another execution of the instruction that has been reordered and executed
			if (windowTail < 2) {
				programCounter++;
			}
		}
		i++;
	}
}

// determines if window[i] can be reordered into window[1] to make a pair with window [0] to enter the pipeline simultaneously
bool FetchStage::registerNameMatch(int i)
{
    // function returns true if a hazard is found
    bool flag = false;
    
    if ((window[0].opcodeString == "end") || (window[0].opcodeString == "NOP"))
        return true;
    
    //    if (window[i].rd == window[0].rd) flag = true; // WAW hazard (If the loop goes until j <= i), then this instruction is redundant
    
    // should be comparing only down to window[1]?
    // Compare window[i] to previous instructions window[0] to window [i-1]
    for (int j = i - 1; j >= 1; j--) {
        
        if ((window[i].rd == window[j].rd) // WAW hazard
            || (window[i].rs == window[j].rd) // RAW hazard
            || (window[i].rt == window[j].rd) // RAW hazard (isn't rt the destination for certain instructions?)
            || (((window[i].rd == window[j].rt) || (window[i].rd == window[j].rs)) // WAR hazard
                && ((j) > 2) // No WAR hazard after reordering if earlier instruction (at j) reads before the later instruction (at i) updates
                )) {
                
                flag = true;
                break;
            }
    }
    return flag;
}

// For the pipeline to stop, an "end" is inserted at the end of the benchmark (when an "end" is detected in the MEM stage) To stay within array borders, three "NOP" are inserted after "end".  "end"/"NOP" is not included in reordering, but enters the window/pipeline to stop the pipeline.

// Returns whether instructions should be executed as a pair

bool FetchStage::reorder(vector<SimulatedInstruction>& simulatedInstructionList)
{
    if (InstructionType::isBranch(window[0].opcode) // Only a single branch may enter a pipeline each cycle.  (Pipeline flush cannot be implemented correctly for a false prediction).
        || window[0].opcodeString == "end" || window[0].opcodeString == "NOP")
		return false; // window[0] needs to enter the pipeline alone
	if ((window[1].rs != window[0].rd) && (window[1].rt != window[0].rd) && (window[1].opcodeString != "end") && (window[1].opcodeString != "NOP")) {
		return true; // no data dependence between [0] and [1]; (end must traverse pipeline as [0])
	}

    // If window[0] can potentially be paired, but the window[1] depends on window[0]
	for (int i = 2; i < windowTail; i++) {
        if (InstructionType::isBranch(window[i].opcode) || window[i].opcodeString == "end" || window[i].opcodeString == "NOP") // cannot be reordered if instruction is a branch or NOP
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


void FetchStage::process(vector<SimulatedInstruction>& simulatedInstructionList, int lastStall, bool branchMisprediction, int branchTarget)
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
            
            if (window[1].reordered) {
                // The pair of instructions are the result of reordering, so the next sequential address simulatedInstructionList[PC+1] will not enter the pipeline in this cycle
                // simulatedInstructionList[PC+1] will be window[0] the next cycle
                programCounter += 1;
            } else {
                programCounter += 2;
            }
            
		} else { // "NOP" is inserted as the second instruction and window[0] enters pipeline alone
			window[0].loopCount = window[0].instructionLocation + upBranch;
			currentInstructionList[0] = window[0];
			currentInstructionList[1] = SimulatedInstruction();
			programCounter++;
		}
		clear_reordered(simulatedInstructionList, programCounter, lastPC); // Clear all the instructions that have entered the pipeline this cycle so that when the program counter branches upper address, the instructions can be executed again. If an instruction's reordered flag is set, it cannot enter the instruction window.

        // Predict not taken policy is implemented, but the branch was taken
		if (branchMisprediction) { // The five stages are processed in reverse order, so the changed PC must not be used by this cycle's fetch stage.  To ensure this, the program counter is only updated after actual issuing is finished using the old program counter.
			int updatedPC = branchTarget;
			upBranch = upBranch + (programCounter - updatedPC);
			simulatedInstructionList[programCounter].reordered = false;
			programCounter = updatedPC;
		}
	}
}
