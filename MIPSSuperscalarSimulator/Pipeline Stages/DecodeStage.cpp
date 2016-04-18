//
//  DecodeStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "DecodeStage.hpp"

DecodeStage::DecodeStage() : readAfterWriteHazard(false) {}

void DecodeStage::process(const RegisterFile& regFile, const vector<SimulatedInstruction>& hazardList, int lastStall) {
	
    if (readAfterWriteHazard || (currentInstructionList.front().opcodeString == "NOP")) {
		return;
	}

	check(hazardList, lastStall);
    
    // Retrieve rs and rt values for the two instructions from the register file
	currentInstructionList[0].rsValue = regFile.getValue(currentInstructionList[0].rs);
	currentInstructionList[0].rtValue = regFile.getValue(currentInstructionList[0].rt);
	currentInstructionList[1].rsValue = regFile.getValue(currentInstructionList[1].rs);
	currentInstructionList[1].rtValue = regFile.getValue(currentInstructionList[1].rt);
}

// Instructions such as SW and LW only have rt or rs and will assign 0 values to rt or rs that is not needed

// Forwarding occurs between the rdValue of an instruction in the MEM stage and an instruction with rsForward(rtForwarding) set at the beginning of the EX stage

// read after write hazards and forwarding possibilities
void DecodeStage::check(const vector<SimulatedInstruction>& hazardList, int lastStall) {
    
    for (int i = 0; i <= 1; i++) {
        
        SimulatedInstruction instruction = currentInstructionList[i];
        
        // lastStall refers to the number of cycles since the last stall
        if (lastStall == 2) {
            
            // Judge 1 is based on hazardList index of 0
            if (instruction.rs == hazardList[0].rd) {
                if (instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rsForward = true;
                    instruction.currentForward.rsForwardDepth = 0;
                } else {
                    instruction.currentForward.rsForward = false;
                }
            }
            
            if (instruction.rt == hazardList[0].rd) {
                if(instruction.loopCount > hazardList[0].loopCount) {
                    instruction.currentForward.rtForward = true;
                    instruction.currentForward.rtForwardDepth = 0;
                } else {
                    instruction.currentForward.rtForward = false;
                }
            }
            
            // Judge 2 is based on hazard list index of 1
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
            if (instruction.currentForward.rsForward || instruction.currentForward.rtForward) {  // In case this instruction has data dependence
                
                // typo??? the innermost if statement may need to be checked against rtForward not rsForward
                if (instruction.currentForward.rsForward) {
                    int index = instruction.currentForward.rsForwardDepth + 2;
                    if (hazardList[index].opcodeString == "LW") {
                        if (instruction.opcodeString == "SW") {
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
                    if (hazardList[index].opcodeString == "LW") {
                        if (instruction.opcodeString == "SW") {
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
