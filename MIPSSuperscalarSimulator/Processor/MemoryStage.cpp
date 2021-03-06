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

    for (SimulatedInstruction const& instruction: currentInstructionList) {
        if (instruction.effectiveAddress != 0) {
            if (instruction.opcode == opcode_sw) {
                cout << "Mem[" << instruction.effectiveAddress << "] = " << instruction.rtValue << endl;
            }
        }
    }

	if (currentInstructionList.front().opcodeString == "NOP") // in case there is RAW hazard detected last cycle
	return;
    
    // Is there a possibility for store word before load word hazard? If not then the two (or three) loops can be combined.
    
    for (SimulatedInstruction& instruction: currentInstructionList) {
        if (instruction.opcode == opcode_lw) {
            // Mem[effectiveAddress] is placed in LMD (load memory data)
            // rdValue is used to indicate LMD
            // rdValue is a software concept that indicates the value should be updated to register file or main memory, it corresponds to the ALUoutput register
            instruction.rdValue = mainMemory.getValue(instruction.effectiveAddress);
        }
    }
    
    // Although instruction is not being modified here, it is still useful to use a reference so that instruction does not need to be copied from currentInstructionList
    for (SimulatedInstruction const& instruction: currentInstructionList) {
        if (instruction.opcode == opcode_sw) {
            // GPR[rt] is placed in Mem[effectiveAddress]
            mainMemory.setValue(instruction.rtValue, instruction.effectiveAddress);
            
            // This line allows for the register value to be updated underneath, but the original decoded rtValue is correct
//            mainMemory.setValue(regfile.getValue(instruction.rt), instruction.effectiveAddress);
        }
    }
    
    
}
