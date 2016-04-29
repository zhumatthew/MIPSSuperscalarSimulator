//
//  DecodeStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "DecodeStage.hpp"

DecodeStage::DecodeStage() : readAfterWriteHazard(false) {}

void DecodeStage::process(RegisterFile const& regFile, vector<SimulatedInstruction> const& hazardList, int lastStall) {
	
    if (readAfterWriteHazard || (currentInstructionList.front().opcodeString == "NOP")) {
		return;
	}

	check(hazardList, lastStall);
    
    // Retrieve rs and rt values for the two instructions from the register file
    for (SimulatedInstruction instruction: currentInstructionList) {
        instruction.rsValue = regFile.getValue(instruction.rs);
        instruction.rtValue = regFile.getValue(instruction.rt);
    }

}

// Instructions such as SW and LW only have rt or rs and will assign 0 values to rt or rs that is not needed

// Forwarding occurs between the rdValue of an instruction in the MEM stage and an instruction with rsForward(rtForwarding) set at the beginning of the EX stage

// read after write hazards and forwarding possibilities
void DecodeStage::check(vector<SimulatedInstruction> const& hazardList, int lastStall) {
    
    // True data dependence (RAW hazard) checking
    
    for (SimulatedInstruction& instruction: currentInstructionList) {
        
        // lastStall refers to the number of cycles since the last stall
        if (lastStall == 2) {
            
            // hazardList[0] and [1] are instructions decoded two cycles ago
            if (instruction.rs == hazardList[0].rd) {
                if (instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rsForward = true;
                    instruction.currentForward.rsForwardDepth = 0;
                } else {
                    instruction.currentForward.rsForward = false;
                }
            }
            
            if (instruction.rt == hazardList[0].rd) {
                if (instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rtForward = true;
                    instruction.currentForward.rtForwardDepth = 0;
                } else {
                    instruction.currentForward.rtForward = false;
                }
            }
            
            if (instruction.rs == hazardList[1].rd) {
                if(instruction.loopCount > hazardList[1].loopCount) {
                    instruction.currentForward.rsForward = true;
                    instruction.currentForward.rsForwardDepth = 1;
                } else {
                    instruction.currentForward.rsForward = false;
                }
            }
            
            if (instruction.rt == hazardList[1].rd) {
                if(instruction.loopCount > hazardList[1].loopCount) {
                    instruction.currentForward.rtForward = true;
                    instruction.currentForward.rtForwardDepth = 1;
                } else {
                    instruction.currentForward.rtForward = false;
                }
            }
        } else {
            
            // hazardList[2] and [3] are the instructions decoded the previous cycle
            if (instruction.rs == hazardList[2].rd) {
                if(instruction.loopCount > hazardList[2].loopCount) {
                    instruction.currentForward.rsForward = true;
                    instruction.currentForward.rsForwardDepth = 0;
                } else {
                    instruction.currentForward.rsForward = false;
                }
            }
            
            if (instruction.rt == hazardList[2].rd) {
                if (instruction.loopCount > hazardList[2].loopCount) {
                    instruction.currentForward.rtForward = true;
                    instruction.currentForward.rtForwardDepth = 0;
                } else {
                    instruction.currentForward.rtForward = false;
                }
            }
            
            if (instruction.rs == hazardList[3].rd) {
                if (instruction.loopCount > hazardList[3].loopCount) {
                    instruction.currentForward.rsForward = true;
                    instruction.currentForward.rsForwardDepth = 1;
                } else {
                    instruction.currentForward.rsForward = false;
                }
            }
            
            if (instruction.rt == hazardList[3].rd) {
                if(instruction.loopCount > hazardList[3].loopCount) {
                    instruction.currentForward.rtForward = true;
                    instruction.currentForward.rtForwardDepth = 1;
                } else {
                    instruction.currentForward.rtForward = false;
                }
            }
            
            // Judge read after write hazard
            if (instruction.currentForward.rsForward || instruction.currentForward.rtForward) {  // In case this instruction is data dependent on previous instructions
                
                // Only LW has "rd" and it is equal to its "rt"
                
                // If the rd of the potential hazard (previous) instruction is this instruction's source register rs or rt and this instruction is SW and the hazard instruction is LW
                // in case these conditions are true, once this instruction (which is a SW) passes the execute stage, it is NOP'ed before it reaches the memory stage, because what it would have wrote into memory is wrong, since it did not get its updated rt or rs value from the previous LW instruction.
                
                // typo??? the innermost if statement may need to be checked against rtForward not rsForward
                if (instruction.currentForward.rsForward) {
                    int index = instruction.currentForward.rsForwardDepth + 2;
                    if (hazardList[index].opcodeString == "lw") {
                        if (instruction.opcodeString == "sw") {
                            if (instruction.currentForward.rsForward) {
                                readAfterWriteHazard = true;
                            } else {
                                readAfterWriteHazard = false;
                            }
                        } else {
                            readAfterWriteHazard = true;
                        }
                    }
                }
                
                if (instruction.currentForward.rtForward) {
                    int index = instruction.currentForward.rtForwardDepth + 2;
                    if (hazardList[index].opcodeString == "lw") {
                        if (instruction.opcodeString == "sw") {
                            if (instruction.currentForward.rsForward) {
                                readAfterWriteHazard = true;
                            } else {
                                readAfterWriteHazard = false;
                            }
                        } else {
                            readAfterWriteHazard = true;
                        }
                    }
                }
            }
            
            // Delayed forwards, last stall != 2
            if (instruction.rs == hazardList[0].rd) {
                if (instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rsDelayedForward = true;
                    instruction.currentForward.rsDelayedForwardDepth = 0;
                } else {
                    instruction.currentForward.rsDelayedForward = false;
                }
            }
            
            if (instruction.rt == hazardList[0].rd) {
                if (instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rtDelayedForward = true;
                    instruction.currentForward.rtDelayedForwardDepth = 0;
                } else {
                    instruction.currentForward.rtDelayedForward = false;
                }
            }
            
            if (instruction.rs == hazardList[1].rd) {
                if (instruction.loopCount > hazardList[1].loopCount) {
                    instruction.currentForward.rsDelayedForward = true;
                    instruction.currentForward.rsDelayedForwardDepth = 1;
                } else {
                    instruction.currentForward.rsDelayedForward = false;
                }
            }
            
            if (instruction.rt == hazardList[1].rd) {
                if (instruction.loopCount > hazardList[1].loopCount) {
                    instruction.currentForward.rtDelayedForward = true;
                    instruction.currentForward.rtDelayedForwardDepth = 1;
                } else {
                    instruction.currentForward.rtDelayedForward = false;
                }
            }
        }
    }
}

// In consideration of forwarding, only the EX stage needs to implement forwarding by first checking the rsForward and rtForward boolean values. The RAW hazard boolean must be false for forwarding to occur, otherwise forwarding must be performed on a cycle when the boolean is false. When performing forwarding, rs, rt, or both rs and rt must be forwarded from the first instruction of the MEM stage (currentInstructionList[0]).

// Rollover appears to refer to moving instructions from one stage to the next.
