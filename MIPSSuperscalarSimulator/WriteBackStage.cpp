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

// Places the destination register value into the destination register of the register file
// returns an increment for the simulator's instruction count
// If the first instruction is NOP or is empty, then the instruction count is not incremented at all.
// If the second instruction in the list is a NOP, the instruction count is incremented by one. Otherwise, it is incremented by two.
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
