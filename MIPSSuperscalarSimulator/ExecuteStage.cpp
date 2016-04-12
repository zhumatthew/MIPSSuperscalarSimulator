//
//  ExecuteStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "ExecuteStage.hpp"

void ExecuteStage::implement(DecodeStage currentDecode, MemoryStage currentMemory, RegisterFile simuRegFile, int lastStall, bool falsePrediction) {
	if (currentDecode.readAfterWriteHazard || (currentInstructionList[0].opcodeString=="NOP"))
		return;
	for (int i = 0; i < 2; i++) {
		if (currentInstructionList[i].currentForward.rsDelayedForward) {
			currentInstructionList[i].rsValue = simuRegFile.getValue(currentInstructionList[i].rs);
		}

		if (currentInstructionList[i].currentForward.rtDelayedForward) {
			currentInstructionList[i].rtValue = simuRegFile.getValue(currentInstructionList[i].rt);
		}

		if (currentInstructionList[i].currentForward.rsForward) {
			int depthIndex = currentInstructionList[i].currentForward.rsForwardDepth;
			currentInstructionList[i].rsValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}

		if (currentInstructionList[i].currentForward.rtForward) {
			int depthIndex = currentInstructionList[i].currentForward.rtForwardDepth;
			currentInstructionList[i].rtValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}

		if (lastStall == 2) {
			if(currentInstructionList[i].currentForward.rsForward)
				currentInstructionList[i].rsValue = simuRegFile.getValue(currentInstructionList[i].rs);

			if(currentInstructionList[i].currentForward.rtForward)
				currentInstructionList[i].rtValue = simuRegFile.getValue(currentInstructionList[i].rt);
		}

		currentInstructionList[i].currentForward.rsDelayedForward = false;
		currentInstructionList[i].currentForward.rsForward = false;
		currentInstructionList[i].currentForward.rtDelayedForward = false;
		currentInstructionList[i].currentForward.rtForward = false;

		if(currentInstructionList[i].opcodeString == "ADD") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue + currentInstructionList[i].rtValue;
		} else if(currentInstructionList[i].opcodeString=="DIV") {
			if(currentInstructionList[i].rtValue == 0)
				return;
			else
				currentInstructionList[i].rdValue = currentInstructionList[i].rsValue / currentInstructionList[i].rtValue;
		} else if(currentInstructionList[i].opcodeString == "SUB") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue - currentInstructionList[i].rtValue;
		} else if(currentInstructionList[i].opcodeString == "MUL") {
			currentInstructionList[i].rdValue = (currentInstructionList[i].rsValue) * (currentInstructionList[i].rtValue);
		} else if(currentInstructionList[i].opcodeString == "LW" || currentInstructionList[i].opcodeString=="SW") {
			currentInstructionList[i].effectiveAddress = currentInstructionList[i].rsValue + currentInstructionList[i].immediate;
		} else if(currentInstructionList[i].opcodeString=="ADDI") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue + currentInstructionList[i].immediate;
		} else if(currentInstructionList[i].opcodeString=="BGEZ" || currentInstructionList[i].opcodeString=="BLEZ" || currentInstructionList[i].opcodeString=="BEQ" || currentInstructionList[i].opcodeString=="J") {
			if((currentInstructionList[i].opcodeString=="BGEZ" && currentInstructionList[i].rsValue >= 0)
					|| (currentInstructionList[i].opcodeString=="BLEZ" && currentInstructionList[i].rsValue <= 0)
					|| (currentInstructionList[i].opcodeString=="BEQ" && (currentInstructionList[i].rsValue == currentInstructionList[i].rtValue))) {
				currentInstructionList[i].branchCondition = true;
			}
			if(currentInstructionList[i].opcodeString=="J")
				currentInstructionList[i].branchCondition = true;
			if(currentInstructionList[i].branchCondition == true) {
				tempPC = currentInstructionList[i].immediate;
				falsePrediction = true;
				currentInstructionList[i].branchCondition = false;
			}
		}
	}
}

int ExecuteStage::getSavedPC(){
	return tempPC;
}
