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
// Returns an increment for the simulator's instruction count
// If the first instruction is NOP or is empty, then the instruction count is not incremented at all.
// If the second instruction in the list is a NOP, the instruction count is incremented by one. Otherwise, it is incremented by two.
// DecodeStage is not used
void WriteBackStage::process(RegisterFile& regFile, DecodeStage currentDecode, int& increment) {

    increment = 0;
	if (currentInstructionList[0].opcodeString == "NOP" || currentInstructionList[0].opcodeString == "nop") {
		return;
	} else {
		if (currentInstructionList[1].opcodeString == "NOP")
            increment = 1;
		else
            increment = 2;
	}
    
    for (SimulatedInstruction instruction: currentInstructionList) {
        regFile.setValue(instruction.rdValue, instruction.rd);
        cout << "r" << instruction.rd << "=" << regFile.getValue(instruction.rd) << endl;
    }
    
}
