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

	if(currentInstructionList[0].effectiveAddress != 0){
        cout << "MemoryAddress [ " << currentInstructionList[0].effectiveAddress << "] = ";
	}
    // currentInstructionList.front();

	if(currentInstructionList[1].effectiveAddress != 0){
        cout << "MemoryAddress [ " << currentInstructionList[1].effectiveAddress << "] = ";
	}

	if(currentInstructionList[0].opcodeString == "NOP" || currentInstructionList[0].opcodeString == "Empty" || currentInstructionList[0].opcodeString == "nop" || currentInstructionList[0].opcodeString == "nop")
	return;

	if(currentInstructionList[0].opcodeString == "LW")
		currentInstructionList[0].rdValue = mmemo.getValue(currentInstructionList[0].effectiveAddress);

	if(currentInstructionList[1].opcodeString == "LW")
		currentInstructionList[1].rdValue = mmemo.getValue(currentInstructionList[1].effectiveAddress);

	if(currentInstructionList[0].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList[0].rt), currentInstructionList[0].effectiveAddress);

	if(currentInstructionList[1].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(currentInstructionList[1].rt), currentInstructionList[1].effectiveAddress);
}
