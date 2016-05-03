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
    
    // Instruction list size includes the one "end" and three "NOP" instructions at the end of the simulated instruction list
    
	while ((i < instructionListSize) && (windowTail < WINDOW_SIZE)) {
		window[windowTail] = simulatedInstructionList[i];
		if (!window[windowTail].reordered) {
			windowTail++;
		} else { // Avoids a repeated execution of the instruction that has been reordered
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
    // flag is set to true if a hazard is found
    bool flag = false;
    
    if ((window[0].opcodeString == "end") || (window[0].opcodeString == "NOP"))
        return true;
    
        if (window[i].rd == window[0].rd) flag = true; // Avoids overwriting from the reordered instruction
    
    // Compare window[i] to previous instructions window[1] to window [i-1]
    for (int j = i - 1; j >= 1; j--) {
        
        if ((window[i].rd == window[j].rd) // WAW hazard
            || (window[i].rs == window[j].rd) // RAW hazard
            || (window[i].rt == window[j].rd) // RAW hazard (rt is the target for certain instructions)
            || (((window[i].rd == window[j].rt) || (window[i].rd == window[j].rs)) // WAR hazard
                && ((j) > 2) // No WAR hazard for reordering [i] to [1] if instruction [j] reads before instruction [i] writes
                )) {
                
                flag = true;
                break;
            }
    }
    return flag;
}

// "end"/"NOP" instructions are not involved in reordering, but enter the window and pipeline to signal the end of execution.

// Returns whether or not instructions should be executed as a pair

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

void FetchStage::clear_reordered(vector<SimulatedInstruction>& simulatedInstructionList, int i, int j)
{   // reset reordered for all instructions that are executed or leaped over this cycle
	for (int k = i - 1; k >= j; k--) {
		simulatedInstructionList[k].reordered = false;
	}
}


void FetchStage::process(vector<SimulatedInstruction>& simulatedInstructionList, int lastStall, bool branchMisprediction, int branchTarget)
{

    int lastPC = programCounter;
	windowMove(simulatedInstructionList);
	bool paired = reorder(simulatedInstructionList);

	if (lastStall == 1) {
		return; // no instruction is fetched on a stall cycle
	} else {
		if (paired) {
            // The actual program counter plus upBranch is where the programCounter would be if there was no branch
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
        
        // Clear all the instructions that have entered the pipeline this cycle so that when the program counter branches upper address, the instructions can be executed again. If an instruction's reordered flag is set, it cannot enter the instruction window.
		clear_reordered(simulatedInstructionList, programCounter, lastPC);

        // Predict not taken policy is implemented, but the branch was taken
		if (branchMisprediction) {
            // The five stages are processed from WB to IF, so the branch target must not be used by the fetch stage for this cycle.  The program counter is updated to the branch target after instructions have already been fetched.
			upBranch = upBranch + (programCounter - branchTarget);
			simulatedInstructionList[programCounter].reordered = false;
			programCounter = branchTarget;            
		}
	}
}
