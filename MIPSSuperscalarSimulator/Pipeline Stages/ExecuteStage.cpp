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

void ExecuteStage::process(DecodeStage currentDecode, MemoryStage currentMemory, const RegisterFile& regFile, int lastStall, bool& branchMisprediction) {
    
    // In case a RAW hazard is detected in last cycle
    if (currentDecode.readAfterWriteHazard || (currentInstructionList.front().opcodeString == "NOP"))
        return;
    
    for (int i = 0; i <= 1; i++) {
        SimulatedInstruction instruction = currentInstructionList[i];
        
        if (instruction.currentForward.rsDelayedForward) {
            instruction.rsValue = regFile.getValue(instruction.rs);
        }
        
        if (instruction.currentForward.rtDelayedForward) {
            instruction.rtValue = regFile.getValue(instruction.rt);
        }
        
        // At the second cycle since the RAW hazard was detected (lastStall == 2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        // Implement forwarding with MEM as source for forwarding
        // Since MEM is processed before EX, the rdValue of the instruction in the memory stage may be the result of a "lw" instruction (is this accounted for by hazards?)
        // Otherwise, the rdValue of the instructions in MEM will be the result of the preceding calculations of the EX stage
        
        if (instruction.currentForward.rsForward) {
            int depthIndex = instruction.currentForward.rsForwardDepth;
            instruction.rsValue = currentMemory.currentInstructionList[depthIndex].rdValue;
        }
        
        if (instruction.currentForward.rtForward) {
            int depthIndex = instruction.currentForward.rtForwardDepth;
            instruction.rtValue = currentMemory.currentInstructionList[depthIndex].rdValue;
        }
        
        // Newly loaded, updated register value is required for the operation, so the register file needs to be accessed again
        // Implement stage sequence is from WB to IF
        
        if (lastStall == 2) {
            if (instruction.currentForward.rsForward)
                instruction.rsValue = regFile.getValue(instruction.rs);
            
            if (instruction.currentForward.rtForward)
                instruction.rtValue = regFile.getValue(instruction.rt);
        }
        
        instruction.currentForward.rsDelayedForward = false;
        instruction.currentForward.rsForward = false;
        instruction.currentForward.rtDelayedForward = false;
        instruction.currentForward.rtForward = false;
        
        /* To be implemented
        // If it is a R-type opcode, then switch on funct
        // Otherwise, switch on opcode
        */
        
        if (instruction.opcodeString == "ADD") {
            instruction.rdValue = instruction.rsValue + instruction.rtValue;
        } else if (instruction.opcodeString == "DIV") {
            if (instruction.rtValue == 0) // cannot divide by 0
                return;
            else
                instruction.rdValue = instruction.rsValue / instruction.rtValue;
        } else if (instruction.opcodeString == "SUB") {
            instruction.rdValue = instruction.rsValue - instruction.rtValue;
        } else if (instruction.opcodeString == "MUL") {
            instruction.rdValue = (instruction.rsValue) * (instruction.rtValue);
        } else if (instruction.opcodeString == "LW" || instruction.opcodeString == "SW") {
            // immediate corresponds to offset for LW/SW instructions
            // Branch instructions involving effective address calculations should consider at which stage condition evaluation, effective address calculation, and program counter changing should be completed.
            instruction.effectiveAddress = instruction.rsValue + instruction.immediate;
        } else if (instruction.opcodeString == "ADDI") {
            instruction.rdValue = instruction.rsValue + instruction.immediate;
        } else if (instruction.opcodeString == "BGEZ" || instruction.opcodeString == "BLEZ" || instruction.opcodeString == "BEQ" || instruction.opcodeString == "J") {
            // Add actions required by branch instructions in this stage
            // The label produced by the assembler is the number for the target instruction in the instruction queue
            // The instruction queue is indexed starting at 0
            if ((instruction.opcodeString == "BGEZ" && instruction.rsValue >= 0)
                || (instruction.opcodeString == "BLEZ" && instruction.rsValue <= 0)
                || (instruction.opcodeString == "BEQ" && (instruction.rsValue == instruction.rtValue))) {
                instruction.branchCondition = true; // branch condition variable will only be tested or changed in the implement function of execution stage
            } // condition evaluation of conditional branch
            if (instruction.opcodeString == "J") // unconditional branch
                instruction.branchCondition = true;
            if (instruction.branchCondition) {
                // Save the PC. just for convenience, actually the target address will be updated to PC (all the five stages share this
                // static field) in this cycle, but every instruction indicated by PC won't be fetched until next cycle's IF stage
                savedProgramCounter = instruction.immediate;
                branchMisprediction = true; // This flag is set so that a bubble is inserted into EX and ID stages in the next cycle
                instruction.branchCondition = false; // Every time after this flag is used, it should be reset to false so that the next time it can be set or reset based on the outcome of the condition evaluation.
            }
            // For this instruction set, the target's absolute address (rather than the relative address to the PC) is assigned to the immediate of the branch instruction. The calculation of the effective address is not needed.
        }
        // In MIPS pipeline, the target address is not known earlier than the branch outcome, there is no advantage for the branch taken strategy. For the branch untaken strategy, branch condition evaluation and PC changing is done at the execution stage of the branch instruction. The target instruciton is fetched at the next cycle and the EX and ID stages' instruction are turned into NOP.
    }
}

int ExecuteStage::getSavedProgramCounter() {
    return savedProgramCounter;
}
