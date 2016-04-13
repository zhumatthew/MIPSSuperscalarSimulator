//
//  MemoryStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MemoryStage.hpp"

using namespace std;

void MemoryStage::implement(MainMemory mmemo, RegisterFile regfile){

	if (currentInstructionList.front().effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList.front().effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList[1].effectiveAddress != 0) {
        cout << "MemoryAddress [ " << currentInstructionList[1].effectiveAddress << "] = " << endl;
	}

	if (currentInstructionList.front().opcodeString == "NOP" || currentInstructionList.front().opcodeString == "Empty" || currentInstructionList[0].opcodeString == "nop" || currentInstructionList.front().opcodeString == "nop")
	return;

	if (currentInstructionList.front().opcodeString == "LW")
		currentInstructionList.front().rdValue = mmemo.getValue(currentInstructionList.front().effectiveAddress);

	if (currentInstructionList[1].opcodeString == "LW")
		currentInstructionList[1].rdValue = mmemo.getValue(currentInstructionList[1].effectiveAddress);

	if (currentInstructionList.front().opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList.front().rt), currentInstructionList.front().effectiveAddress);

	if (currentInstructionList[1].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList[1].rt), currentInstructionList[1].effectiveAddress);
}
