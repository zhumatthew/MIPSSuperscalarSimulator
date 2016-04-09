//
//  MemoryStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MemoryStage.hpp"

using namespace std;

MemoryStage::MemoryStage() {
    
}

void MemoryStage::implement(MainMemory mmemo, RegisterFile regfile){

	if(this->currentInstructionList[0].effectiveAddress != 0){
	cout << "MemoryAddress [ " + this->currentInstructionList[0].effectiveAddress + "] = ";
	}

	if(this->currentInstructionList[1].effectiveAddress != 0){
	cout << "MemoryAddress [ " + this->currentInstructionList[1].effectiveAddress + "] = ";
	}

	if(this->currentInstructionList[0].opcodeString == "NOP" || this->currentInstructionList[0].opcodeString == "Empty" || this->currentInstructionList[0].opcodeString == "nop" || this->currentInstructionList[0].opcodeString == "nop")
	return;

	if(this->currentInstructionList[0].opcodeString == "LW")
		this->currentInstructionList[0].rdValue = mmemo.getValue(this->currentInstructionList[0].effectiveAddress);

	if(this->currentInstructionList[1].opcodeString == "LW")
		this->currentInstructionList[1].rdValue = mmemo.getValue(this->currentInstructionList[1].effectiveAddress);

	if(this->currentInstructionList[0].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(this->currentInstructionList[0].rt), this->currentInstructionList[0].effectiveAddress);

	if(this->currentInstructionList[1].opcodeString == "SW")
		mmemo.putValue(regfile.getValue(this->currentInstructionList[1].rt), this->currentInstructionList[1].effectiveAddress);
}
