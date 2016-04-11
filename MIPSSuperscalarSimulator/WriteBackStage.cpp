//
//  WriteBackStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "WriteBackStage.hpp"
#include <iostream>

using namespace std;

int WriteBackStage::implement(RegisterFile regFile, DecodeStage currentDecode){

    int increment;
	if(this->currentInstructionList[0].opcodeString == "NOP" || this->currentInstructionList[0].opcodeString == "nop" || this->currentInstructionList[0].opcodeString == "Empty") {
		return increment;
	} else {
		if(this->currentInstructionList[1].opcodeString == "NOP")
//			sim.instrCount++;
            increment = 1;
		else
//			sim.instrCount = sim.instrCount + 2;
            increment = 2;
	}
	regFile.putValue(this->currentInstructionList[0].rdValue, this->currentInstructionList[0].rd);
	regFile.putValue(this->currentInstructionList[1].rdValue, this->currentInstructionList[1].rd);
    cout << "r" << this->currentInstructionList[0].rd << "=" << regFile.getValue(this->currentInstructionList[0].rd);
    cout << "r" << this->currentInstructionList[1].rd << "=" << regFile.getValue(this->currentInstructionList[1].rd);
    return increment;
}
