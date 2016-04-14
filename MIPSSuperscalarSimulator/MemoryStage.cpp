//
//  MemoryStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MemoryStage.hpp"

using namespace std;

// In the memory stage, load word and store word operations are performed
void MemoryStage::implement(MainMemory mmemo, RegisterFile regfile) {

	if (currentInstructionList.front().effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList.front().effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList[1].effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList[1].effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList.front().opcodeString == "NOP" || currentInstructionList.front().opcodeString == "Empty" ||
        currentInstructionList.front().opcodeString == "nop") // in case there is RAW hazard detected last cycle
	return;

    // Mem[effectiveAddress] is placed in LMD (load memory data)
    // rdValue is used to indicate LMD
    // rdValue is a software concept that indicates value should be updated to register file or main memory, it is responding to ALUoutput register
	if (currentInstructionList.front().opcodeString == "LW")
		currentInstructionList.front().rdValue = mmemo.getValue(currentInstructionList.front().effectiveAddress);

	if (currentInstructionList[1].opcodeString == "LW")
		currentInstructionList[1].rdValue = mmemo.getValue(currentInstructionList[1].effectiveAddress);

    // GPR[rs] is placed in Mem[effectiveAddress]
	if (currentInstructionList.front().opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList.front().rt), currentInstructionList.front().effectiveAddress);

	if (currentInstructionList[1].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList[1].rt), currentInstructionList[1].effectiveAddress);
    
    
}
