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
void MemoryStage::process(MainMemory& mainMemory, RegisterFile& regfile) {

	if (currentInstructionList[0].effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList[0].effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList[1].effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList[1].effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList.front().opcodeString == "NOP") // in case there is RAW hazard detected last cycle
	return;

    // Mem[effectiveAddress] is placed in LMD (load memory data)
    // rdValue is used to indicate LMD
    // rdValue is a software concept that indicates value should be updated to register file or main memory, it is responding to ALUoutput register
	if (currentInstructionList[0].opcodeString == "lw")
		currentInstructionList[0].rdValue = mainMemory.getValue(currentInstructionList[0].effectiveAddress);

	if (currentInstructionList[1].opcodeString == "lw")
		currentInstructionList[1].rdValue = mainMemory.getValue(currentInstructionList[1].effectiveAddress);

    // GPR[rs] is placed in Mem[effectiveAddress]
	if (currentInstructionList[0].opcodeString == "sw")
		mainMemory.setValue(regfile.getValue(currentInstructionList[0].rt), currentInstructionList[0].effectiveAddress);

	if (currentInstructionList[1].opcodeString == "sw")
		mainMemory.setValue(regfile.getValue(currentInstructionList[1].rt), currentInstructionList[1].effectiveAddress);
    
    
}
