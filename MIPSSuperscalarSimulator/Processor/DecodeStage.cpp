//
//  DecodeStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "DecodeStage.hpp"

DecodeStage::DecodeStage() : loadDataDependence(false) {}

void DecodeStage::process(RegisterFile const& regFile, vector<SimulatedInstruction> const& hazardList, int lastStall) {
	
    if (loadDataDependence || (currentInstructionList.front().opcodeString == "NOP")) {
		return;
	}

	check(hazardList, lastStall);
    
    // Retrieve rs and rt values for the instructions from the register file
    for (SimulatedInstruction& instruction: currentInstructionList) {
        instruction.rsValue = regFile.getValue(instruction.rs);
        instruction.rtValue = regFile.getValue(instruction.rt);
    }
}

// read after write hazards and forwarding possibilities
void DecodeStage::check(vector<SimulatedInstruction> const& hazardList, int lastStall) {
    
    // Check for true data dependence (RAW hazard)
    
    for (SimulatedInstruction& instruction: currentInstructionList) {
        
        if (instruction.opcodeString == "NOP") return;
        
        // lastStall refers to the number of cycles since the last stall
        if (lastStall == 2) {
            
            for (int i = 0; i <= 1; i++) {
                SimulatedInstruction hazard = hazardList[i];
                
                if (instruction.rs == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rsForward = true;
                        instruction.currentForward.rsForwardDepth = i;
                    } else {
                        instruction.currentForward.rsForward = false;
                    }
                }
                
                if (instruction.rt == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rtForward = true;
                        instruction.currentForward.rtForwardDepth = i;
                    } else {
                        instruction.currentForward.rtForward = false;
                    }
                }
               
            }
        } else {
            
            // hazardList[2] and [3] are the instructions decoded the previous cycle
            for (int i = 2; i <= 3; i++) {
                SimulatedInstruction hazard = hazardList[i];
                
                if (instruction.rs == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rsForward = true;
                        instruction.currentForward.rsForwardDepth = i-2;
                    } else {
                        instruction.currentForward.rsForward = false;
                    }
                }
                
                if (instruction.rt == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rtForward = true;
                        instruction.currentForward.rtForwardDepth = i-2;
                    } else {
                        instruction.currentForward.rtForward = false;
                    }
                }
            }

            if (instruction.currentForward.rsForward || instruction.currentForward.rtForward) {
                // LW has "rd" and it is equal to its "rt"
                
                // If previous hazard instruction is lw, and there is a RAW hazard, it is impossible to forward, because the hazard instruction cannot forward from EX to EX. lw only obtains the final rd value in the memory stage
                
                if (instruction.currentForward.rsForward) {
                    int index = instruction.currentForward.rsForwardDepth + 2;
                    if (hazardList[index].opcodeString == "lw") {
                        loadDataDependence = true;
                    }
                }
                
                // All instructions that are dependent on an 'lw' that was decoded the previous cycle
                // must be NOPed because 'lw' only obtains values after MEM, when it is too late to forward.
                // 'sw' calculates its effective address using rs in EX, but does not use rt until MEM (immediately after lw has passed MEM)
                // Forwarding from MEM to MEM?
                if (instruction.currentForward.rtForward) {
                    int index = instruction.currentForward.rtForwardDepth + 2;
                    if (hazardList[index].opcodeString == "lw") {
                        if (instruction.opcodeString == "sw") {
                            if (instruction.currentForward.rsForward) {
                                loadDataDependence = true;
                            } else {
                                loadDataDependence = false;
                            }
                        } else {
                            loadDataDependence = true;
                        }
                    }
                }
            }
            
            // Delayed forwards
            // hazardList[0] and [1] are instructions decoded two cycles ago
            for (int i = 0; i <= 1; i++) {
                SimulatedInstruction hazard = hazardList[i];
                if (instruction.rs == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rsDelayedForward = true;
                        instruction.currentForward.rsDelayedForwardDepth = i;
                    } else {
                        instruction.currentForward.rsDelayedForward = false;
                    }
                }
                
                if (instruction.rt == hazard.rd) {
                    if (instruction.loopCount > hazard.loopCount) {
                        instruction.currentForward.rtDelayedForward = true;
                        instruction.currentForward.rtDelayedForwardDepth = i;
                    } else {
                        instruction.currentForward.rtDelayedForward = false;
                    }
                }
            }
            
        }
    }
}