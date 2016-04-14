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

// Places the destination register value into the destination register
// returns an increment for the simulator's instruction count
int WriteBackStage::implement(RegisterFile& regFile, DecodeStage currentDecode){

    int increment = 0;
	if (currentInstructionList[0].opcodeString == "NOP" || currentInstructionList[0].opcodeString == "nop" || currentInstructionList[0].opcodeString == "Empty") {
		return increment;
	} else {
		if (currentInstructionList[1].opcodeString == "NOP")
//			sim.instrCount++;
            increment = 1;
		else
//			sim.instrCount = sim.instrCount + 2;
            increment = 2;
	}
	regFile.putValue(currentInstructionList[0].rdValue, currentInstructionList[0].rd);
	regFile.putValue(currentInstructionList[1].rdValue, currentInstructionList[1].rd);
    cout << "r" << currentInstructionList[0].rd << "=" << regFile.getValue(currentInstructionList[0].rd) << endl;
    cout << "r" << currentInstructionList[1].rd << "=" << regFile.getValue(currentInstructionList[1].rd) << endl;
    return increment;
}
