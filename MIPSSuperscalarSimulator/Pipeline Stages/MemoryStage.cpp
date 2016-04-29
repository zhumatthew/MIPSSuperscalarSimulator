//
//  MemoryStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MemoryStage.hpp"

using namespace std;

// In the memory stage, load word and store word operations are performed
void MemoryStage::process(MainMemory& mainMemory, RegisterFile& regfile) {

    for (SimulatedInstruction& instruction: currentInstructionList) {
        if (instruction.effectiveAddress != 0) {
            cout << "Mem[" << instruction.effectiveAddress << "] = " << endl;
        }
    }

	if (currentInstructionList.front().opcodeString == "NOP") // in case there is RAW hazard detected last cycle
	return;
    
    // Is there a possibility for store word before load word hazard? If not then the two (or three) loops can be combined.
    
    for (SimulatedInstruction& instruction: currentInstructionList) {
        if (instruction.opcode == opcode_lw) {
            // Mem[effectiveAddress] is placed in LMD (load memory data)
            // rdValue is used to indicate LMD
            // rdValue is a software concept that indicates value should be updated to register file or main memory, it corresponds to the ALUoutput register
            mainMemory.setValue(regfile.getValue(instruction.rt), instruction.effectiveAddress);
        }
    }
    
    for (SimulatedInstruction& instruction: currentInstructionList) {
        if (instruction.opcode == opcode_sw) {
            // GPR[rt] is placed in Mem[effectiveAddress]
            mainMemory.setValue(regfile.getValue(instruction.rt), instruction.effectiveAddress);
        }
    }
    
    
}
