//
//  ExecuteStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "ExecuteStage.hpp"

ExecuteStage::ExecuteStage() {
	Stage::Stage();
}

void ExecuteStage::implement(vector<SimulationInstruction> simulationInstructionList, DecodeStage currentDecode, MemoryStage currentMemory, Simulator currentSimulator) {
	if(currentDecode.readAfterWriteHazard || (this->currentInstructionList[0].opcodeString=="NOP"))
		return;
	for(int i = 0; i < 2; i++) {
		if(this->currentInstructionList[i].currentForward->rsDelayedForward) {
			this->currentInstructionList[i].rsValue = currentSimulator.simuRegFile.getValue(currentInstructionList[i].rs);
		}

		if(this->currentInstructionList[i].currentForward->rtDelayedForward) {
			this->currentInstructionList[i].rtValue = currentSimulator.simuRegFile.getValue(currentInstructionList[i].rt);
		}

		if(this->currentInstructionList[i].currentForward->rsForward) {
			int depthIndex = this->currentInstructionList[i].currentForward->rsForwardDepth;
			this->currentInstructionList[i].rsValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}

		if(this->currentInstructionList[i].currentForward->rtForward) {
			int depthIndex = this->currentInstructionList[i].currentForward->rtForwardDepth;
			this->currentInstructionList[i].rtValue = currentMemory.currentInstructionList[depthIndex].rdValue;
		}

		if( currentSimulator.lastStall == 2) {
			if(this->currentInstructionList[i].currentForward->rsForward)
				this->currentInstructionList[i].rsValue = currentSimulator.simuRegFile.getValue(currentInstructionList[i].rs);

			if(this->currentInstructionList[i].currentForward->rtForward)
				this->currentInstructionList[i].rtValue = currentSimulator.simuRegFile.getValue(currentInstructionList[i].rt);
		}

		this->currentInstructionList[i].currentForward->rsDelayedForward = false;
		this->currentInstructionList[i].currentForward->rsForward = false;
		this->currentInstructionList[i].currentForward->rtDelayedForward = false;
		this->currentInstructionList[i].currentForward->rtForward = false;

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
			if((currentInstructionList[i].opcodeString=="BGEZ" && this->currentInstructionList[i].rsValue >= 0)
					|| (currentInstructionList[i].opcodeString=="BLEZ" && this->currentInstructionList[i].rsValue <= 0)
					|| (currentInstructionList[i].opcodeString=="BEQ" && (this->currentInstructionList[i].rsValue == this->currentInstructionList[i].rtValue))) {
				this->currentInstructionList[i].branchCondition = true;
			}
			if(currentInstructionList[i].opcodeString=="J")
				this->currentInstructionList[i].branchCondition = true;
			if(this->currentInstructionList[i].branchCondition == true) {
				tempPC = this->currentInstructionList[i].immediate;
				currentSimulator.falsePrediction = true;
				this->currentInstructionList[i].branchCondition = false;
			}
		}
	}
}

int ExecuteStage::getSavedPC(){
	return this->tempPC;
}
