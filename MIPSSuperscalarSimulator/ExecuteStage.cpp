//
//  ExecuteStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "ExecuteStage.hpp"

// example instruction
// $d = $s + $t

void ExecuteStage::implement(DecodeStage currentDecode, MemoryStage currentMemory, RegisterFile simuRegFile, int lastStall, bool falsePrediction) {
    // In case a RAW hazard is detected in last cycle
	if (currentDecode.readAfterWriteHazard || (currentInstructionList.front().opcodeString=="NOP"))
		return;
	for (int i = 0; i <= 1; i++) {
		if (currentInstructionList[i].currentForward.rsDelayedForward) {
			currentInstructionList[i].rsValue = simuRegFile.getValue(currentInstructionList[i].rs);
		}

		if (currentInstructionList[i].currentForward.rtDelayedForward) {
			currentInstructionList[i].rtValue = simuRegFile.getValue(currentInstructionList[i].rt);
		}
        
        // At the second cycle since the RAW hazard was detected (lastStall==2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        // Implement forwarding with MEM as origin stage (origin = source?)

		if (currentInstructionList[i].currentForward.rsForward) {
			int depthIndex = currentInstructionList[i].currentForward.rsForwardDepth;
			currentInstructionList[i].rsValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}

		if (currentInstructionList[i].currentForward.rtForward) {
			int depthIndex = currentInstructionList[i].currentForward.rtForwardDepth;
			currentInstructionList[i].rtValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}
        
        // Newly loaded, updated register value is required for the operation, so the register file needs to be accessed again
        // Implement stage sequence is from WB to IF

		if (lastStall == 2) {
			if (currentInstructionList[i].currentForward.rsForward)
				currentInstructionList[i].rsValue = simuRegFile.getValue(currentInstructionList[i].rs);

			if (currentInstructionList[i].currentForward.rtForward)
				currentInstructionList[i].rtValue = simuRegFile.getValue(currentInstructionList[i].rt);
		}

		currentInstructionList[i].currentForward.rsDelayedForward = false;
		currentInstructionList[i].currentForward.rsForward = false;
		currentInstructionList[i].currentForward.rtDelayedForward = false;
		currentInstructionList[i].currentForward.rtForward = false;

		if (currentInstructionList[i].opcodeString == "ADD") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue + currentInstructionList[i].rtValue;
		} else if (currentInstructionList[i].opcodeString == "DIV") {
			if (currentInstructionList[i].rtValue == 0)
				return;
			else
				currentInstructionList[i].rdValue = currentInstructionList[i].rsValue / currentInstructionList[i].rtValue;
		} else if (currentInstructionList[i].opcodeString == "SUB") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue - currentInstructionList[i].rtValue;
		} else if (currentInstructionList[i].opcodeString == "MUL") {
			currentInstructionList[i].rdValue = (currentInstructionList[i].rsValue) * (currentInstructionList[i].rtValue);
		} else if (currentInstructionList[i].opcodeString == "LW" || currentInstructionList[i].opcodeString == "SW") {
			currentInstructionList[i].effectiveAddress = currentInstructionList[i].rsValue + currentInstructionList[i].immediate;
		} else if (currentInstructionList[i].opcodeString == "ADDI") {
			currentInstructionList[i].rdValue = currentInstructionList[i].rsValue + currentInstructionList[i].immediate;
		} else if (currentInstructionList[i].opcodeString == "BGEZ" || currentInstructionList[i].opcodeString == "BLEZ" || currentInstructionList[i].opcodeString == "BEQ" || currentInstructionList[i].opcodeString == "J") {
			if((currentInstructionList[i].opcodeString == "BGEZ" && currentInstructionList[i].rsValue >= 0)
					|| (currentInstructionList[i].opcodeString == "BLEZ" && currentInstructionList[i].rsValue <= 0)
					|| (currentInstructionList[i].opcodeString == "BEQ" && (currentInstructionList[i].rsValue == currentInstructionList[i].rtValue))) {
				currentInstructionList[i].branchCondition = true;
			}
			if (currentInstructionList[i].opcodeString == "J")
				currentInstructionList[i].branchCondition = true;
			if (currentInstructionList[i].branchCondition == true) {
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
