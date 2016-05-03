//
//  ExecuteStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "ExecuteStage.hpp"
#include "../Assembler/InstructionType.hpp"

void ExecuteStage::process(DecodeStage currentDecode, MemoryStage currentMemory, RegisterFile const& regFile, int lastStall, bool& branchMisprediction) {
    
    // In case a load data dependence is detected in last cycle
    if (currentDecode.loadDataDependence || (currentInstructionList.front().opcodeString == "NOP"))
        return;
    
    // instruction must be a reference since rdValue is written to
    for (SimulatedInstruction& instruction: currentInstructionList) {
        
        // Simulator does not include pipeline registers
        // Forwarding from MEM to EX (instructions that are two cycles apart) is accomplished by simply reading registers because the writeback stage is processed before the decode stage (reverse order from WB to IF)
        // Delayed forward indicates that the instruction's read of the register is delayed until
        // the execute stage (after the instruction two cycles ahead has already written to registers)
        
        // MEM to EX (delayed)
        if (instruction.currentForward.rsDelayedForward) {
            instruction.rsValue = regFile.getValue(instruction.rs);
        }
        
        // MEM to EX (delayed)
        if (instruction.currentForward.rtDelayedForward) {
            instruction.rtValue = regFile.getValue(instruction.rt);
        }
        
        // At the second cycle since the load data dependence was detected (lastStall == 2), a NOP needs to be inserted into the MEM stage, but this can lead to unsuccessful forwarding from instructions in MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        // Forwarding with instructions in MEM as source for forwarding
        // The rdValue of the instructions in MEM will be the result of the calculations of the EX stage of the preceding cycle
        
        // Forward from instructions that are one cycle ahead
        // EX to EX
        if (instruction.currentForward.rsForward) {
            int depthIndex = instruction.currentForward.rsForwardDepth;
            instruction.rsValue = currentMemory.currentInstructionList[depthIndex].rdValue;
        }
        
        // EX to EX
        if (instruction.currentForward.rtForward) {
            int depthIndex = instruction.currentForward.rtForwardDepth;
            instruction.rtValue = currentMemory.currentInstructionList[depthIndex].rdValue;
        }
        
        // Stages are processed from WB to IF
        // Newly loaded, updated register value is required for the operation, so the register file needs to be accessed again. Instruction that has passed through the pipeline two cycles ago has already written back to the registers after the writeback stage for this cycle has been processed.
        
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
        
        switch (instruction.opcode) {
            case opcode_rtype:
                switch (instruction.funct) {
                    case funct_add:
                        instruction.rdValue = instruction.rsValue + instruction.rtValue;
                        break;
                    case funct_sub:
                        instruction.rdValue = instruction.rsValue - instruction.rtValue;
                        break;
                    case funct_mult:
                        instruction.rdValue = (instruction.rsValue) * (instruction.rtValue);
                        break;
                    case funct_div:
                        if (instruction.rtValue == 0) // cannot divide by 0
                            return;
                        else
                            instruction.rdValue = instruction.rsValue / instruction.rtValue;
                        break;
                    default:
                        break;
                }
                break;
            case opcode_lw:
            case opcode_sw:
                // immediate is the address offset for LW/SW instructions
                instruction.effectiveAddress = instruction.rsValue + instruction.immediate;
                break;
            case opcode_addi:
                instruction.rdValue = instruction.rsValue + instruction.immediate;
                break;
                
                // Outcome of the branch is only known after the execute stage
                // The label read by the assembler determines the address for the target instruction in the instruction queue
                // the instruction queue is indexed starting at 0
            case opcode_bgtz:
                instruction.branchCondition = instruction.rsValue > 0 ? true : false;
                break;
            case opcode_blez:
                instruction.branchCondition = instruction.rsValue <= 0 ? true : false;
                break;
            case opcode_beq:
                instruction.branchCondition = instruction.rsValue == instruction.rtValue ? true : false;
                break;
                
                // Unconditional branch
            case opcode_j:
                instruction.branchCondition = true;
                break;
            default:
                break;
                
        }
        
        // In a MIPS pipeline, the target address is not determined earlier than the branch outcome, so predicting branches to always be taken is not advantageous.
        // For this simulator, the target's absolute address (rather than the address relative to the PC) is assigned to be the target of the branch instruction. The calculation of the effective address is not needed.
        if (instruction.branchCondition) {
            // Fetching from the branch target begins in the next cycle
            branchTarget = instruction.immediate;
            // This flag is set for instructions in EX and DE to be NOP'ed in the next cycle
            branchMisprediction = true;
            instruction.branchCondition = false; // reset flag
        }
    }
}

int ExecuteStage::getBranchTarget() {
    return branchTarget;
}
