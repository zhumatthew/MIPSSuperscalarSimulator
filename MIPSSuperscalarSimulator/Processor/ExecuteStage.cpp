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
    
    // In case a RAW hazard is detected in last cycle
    if (currentDecode.readAfterWriteHazard || (currentInstructionList.front().opcodeString == "NOP"))
        return;
    
    // instruction must be a reference since rdValue is written to
    for (SimulatedInstruction& instruction: currentInstructionList) {
        
        // Simulator does not include pipeline registers
        // Forwarding from MEM to EX is accomplished by simply reading registers because the writeback stage is processed before the decode stage (reverse order from WB to IF)
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
        
        // At the second cycle since the RAW hazard was detected (lastStall == 2), a NOP needs to be inserted into the MEM stage, but this can lead to the unsuccessful forwarding with an origin stage of MEM since the information in MEM is discarded before it is forwarded to the execution stage of the same cycle
        
        // Implement forwarding with instructions in MEM as source for forwarding
        // Since MEM is processed before EX, the rdValue of the instruction in the memory stage may be the result of a "lw" instruction (accounted for by hazards?)
        // Otherwise, the rdValue of the instructions in MEM will be the result of the preceding calculations of the EX stage
        
        // Instructions that are one cycle ahead need to be forwarded
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
        
        // Newly loaded, updated register value is required for the operation, so the register file needs to be accessed again. Instruction that has passed through the pipeline two cycles ago has already written back to the registers after the writeback stage for this cycle has been processed.
        // Stages are processed from WB to IF
        
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
                // Branch instructions involving effective address calculations should consider at which stage condition evaluation, effective address calculation, and program counter changing should be completed.
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
        
        if (instruction.branchCondition) {
            // Save the PC. just for convenience, actually the target address will be updated to PC (all the five stages share this
            // static field) in this cycle, but every instruction indicated by PC won't be fetched until next cycle's IF stage
            // The immediate value can be the absolute address specified by a label
            branchTarget = instruction.immediate;
            branchMisprediction = true; // This flag is set so that a bubble is inserted into EX and ID stages in the next cycle
            instruction.branchCondition = false; // Every time after this flag is used, it should be reset to false so that the next time it can be set or reset based on the outcome of the condition evaluation.
        }
        
        // For this instruction set, the target's absolute address (rather than the relative address to the PC) is assigned to the immediate of the branch instruction. The calculation of the effective address is not needed.
        
        // In a MIPS pipeline, the target address is not known earlier than the branch outcome and there is no advantage for the branch taken strategy. For the branch untaken strategy, branch condition evaluation and PC changing is done at the execution stage of the branch instruction. The target instruciton is fetched at the next cycle and the EX and ID stages' instructions are turned into NOP.
    }
}

int ExecuteStage::getBranchTarget() {
    return branchTarget;
}
