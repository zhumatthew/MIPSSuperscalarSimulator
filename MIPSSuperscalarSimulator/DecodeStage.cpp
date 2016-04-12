//
//  DecodeStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "DecodeStage.hpp"

DecodeStage::DecodeStage() : readAfterWriteHazard(false) {}

void DecodeStage::implement(MainMemory mmem, RegisterFile regm, vector<SimulationInstruction> hazardList, int lastStall) {
	
    if (readAfterWriteHazard || (currentInstructionList[0].opcodeString == "NOP")) {
		return;
	}

	check(hazardList, lastStall);
	currentInstructionList[0].rsValue = regm.getValue(currentInstructionList[0].rs);
	currentInstructionList[0].rtValue = regm.getValue(currentInstructionList[0].rt);
	currentInstructionList[1].rsValue = regm.getValue(currentInstructionList[1].rs);
	currentInstructionList[1].rtValue = regm.getValue(currentInstructionList[1].rt);
}

// Instructions such as SW and LW only have rt or rs and will assign 0 values to rt or rs that is not needed

// Forwarding occurs between the rdValue of an instruction in the MEM stage and an instruction with rsForward(rtForwarding) set at the beginning of the EX stage


// read after write hazards and forwarding possibilities
void DecodeStage::check(vector<SimulationInstruction> hazardList, int lastStall) {
	if (lastStall == 2) {
        
        // Judge 1 is based on hazardList index of 0
		if (currentInstructionList[0].rs == hazardList[0].rd) {
			if (currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				currentInstructionList[0].currentForward.rsForward = true;
				currentInstructionList[0].currentForward.rsForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[0].rt == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				currentInstructionList[0].currentForward.rtForward = true;
				currentInstructionList[0].currentForward.rtForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rtForward = false;
			}
		}
        
        // Judge 2 is based on hazard list index of 2
		if (currentInstructionList[0].rs == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				currentInstructionList[0].currentForward.rsForward = true;
				currentInstructionList[0].currentForward.rsForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rsForward = false;
			}
		}

		if(currentInstructionList[0].rt == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				currentInstructionList[0].currentForward.rtForward = true;
				currentInstructionList[0].currentForward.rtForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rtForward = false;
			}
		}
	} else {
		if (currentInstructionList[0].rs == hazardList[2].rd) {
			if(currentInstructionList[0].loopCount > hazardList[2].loopCount) {
				currentInstructionList[0].currentForward.rsForward = true;
				currentInstructionList[0].currentForward.rsForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[0].rt == hazardList[2].rd) {
			if(currentInstructionList[0].loopCount > hazardList[2].loopCount) {
				currentInstructionList[0].currentForward.rtForward = true;
				currentInstructionList[0].currentForward.rtForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rtForward = false;
			}
		}

		if (currentInstructionList[0].rs == hazardList[3].rd) {
			if(currentInstructionList[0].loopCount > hazardList[3].loopCount) {
				currentInstructionList[0].currentForward.rsForward = true;
				currentInstructionList[0].currentForward.rsForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[0].rt == hazardList[3].rd) {
			if(currentInstructionList[0].loopCount > hazardList[3].loopCount) {
				currentInstructionList[0].currentForward.rtForward = true;
				currentInstructionList[0].currentForward.rtForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rtForward = false;
			}
		}

		if (currentInstructionList[0].currentForward.rsForward || currentInstructionList[0].currentForward.rtForward) {

			if(currentInstructionList[0].currentForward.rsForward) {
				int index = currentInstructionList[0].currentForward.rsForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (currentInstructionList[0].opcodeString == "SW") {
						if (currentInstructionList[0].currentForward.rsForward == true) {
							readAfterWriteHazard = true;
						} else {
							readAfterWriteHazard = false;
						}
					} else {
						readAfterWriteHazard = true;
					}
				}
			}

			if(currentInstructionList[0].currentForward.rtForward) {
				int index = currentInstructionList[0].currentForward.rtForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (currentInstructionList[0].opcodeString == "SW") {
						if (currentInstructionList[0].currentForward.rsForward == true) {
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

		if (currentInstructionList[0].rs == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				currentInstructionList[0].currentForward.rsDelayedForward = true;
				currentInstructionList[0].currentForward.rsDelayForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rsDelayedForward = false;
			}
		}

		if (currentInstructionList[0].rt == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				currentInstructionList[0].currentForward.rtDelayedForward = true;
				currentInstructionList[0].currentForward.rtDelayForwardDepth = 0;
			} else {
				currentInstructionList[0].currentForward.rtDelayedForward = false;
			}
		}

		if (currentInstructionList[0].rs == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				currentInstructionList[0].currentForward.rsDelayedForward = true;
				currentInstructionList[0].currentForward.rsDelayForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rsDelayedForward = false;
			}
		}

		if (currentInstructionList[0].rt == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				currentInstructionList[0].currentForward.rtDelayedForward = true;
				currentInstructionList[0].currentForward.rtDelayForwardDepth = 1;
			} else {
				currentInstructionList[0].currentForward.rtDelayedForward = false;
			}
		}
	}

	if (lastStall == 2) {

		if (currentInstructionList[1].rs == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				currentInstructionList[1].currentForward.rsForward = true;
				currentInstructionList[1].currentForward.rsForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[1].rt == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				currentInstructionList[1].currentForward.rtForward = true;
				currentInstructionList[1].currentForward.rtForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rtForward = false;
			}
		}

		if (currentInstructionList[1].rs == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				currentInstructionList[1].currentForward.rsForward = true;
				currentInstructionList[1].currentForward.rsForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rsForward = false;
			}
		}

		if(currentInstructionList[1].rt == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				currentInstructionList[1].currentForward.rtForward = true;
				currentInstructionList[1].currentForward.rtForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rtForward = false;
			}
		}

	} else {
		if (currentInstructionList[1].rs == hazardList[2].rd) {
			if(currentInstructionList[1].loopCount > hazardList[2].loopCount) {
				currentInstructionList[1].currentForward.rsForward = true;
				currentInstructionList[1].currentForward.rsForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[1].rt == hazardList[2].rd) {
			if(currentInstructionList[1].loopCount > hazardList[2].loopCount) {
				currentInstructionList[1].currentForward.rtForward = true;
				currentInstructionList[1].currentForward.rtForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rtForward = false;
			}
		}

		if (currentInstructionList[1].rs == hazardList[3].rd) {
			if(currentInstructionList[1].loopCount > hazardList[3].loopCount){
				currentInstructionList[1].currentForward.rsForward = true;
				currentInstructionList[1].currentForward.rsForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rsForward = false;
			}
		}

		if (currentInstructionList[1].rt == hazardList[3].rd) {
			if(currentInstructionList[1].loopCount > hazardList[3].loopCount) {
				currentInstructionList[1].currentForward.rtForward = true;
				currentInstructionList[1].currentForward.rtForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rtForward = false;
			}
		}

		if (currentInstructionList[1].currentForward.rsForward || currentInstructionList[1].currentForward.rtForward) {
			if(currentInstructionList[1].currentForward.rsForward) {
				int index = currentInstructionList[1].currentForward.rsForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (currentInstructionList[1].opcodeString == "SW") {
						if (currentInstructionList[1].currentForward.rsForward == true) {
							readAfterWriteHazard = true;
						} else {
							readAfterWriteHazard = false;
						}
					} else {
						readAfterWriteHazard = true;
					}
				}
			}

			if(currentInstructionList[1].currentForward.rtForward) {
				int index = currentInstructionList[1].currentForward.rtForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (currentInstructionList[1].opcodeString == "SW") {
						if (currentInstructionList[1].currentForward.rsForward == true) {
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

		if (currentInstructionList[1].rs == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				currentInstructionList[1].currentForward.rsDelayedForward = true;
				currentInstructionList[1].currentForward.rsDelayForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rsDelayedForward = false;
			}
		}

		if (currentInstructionList[1].rt == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				currentInstructionList[1].currentForward.rtDelayedForward = true;
				currentInstructionList[1].currentForward.rtDelayForwardDepth = 0;
			} else {
				currentInstructionList[1].currentForward.rtDelayedForward = false;
			}
		}

		if (currentInstructionList[1].rs == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				currentInstructionList[1].currentForward.rsDelayedForward = true;
				currentInstructionList[1].currentForward.rsDelayForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rsDelayedForward = false;
			}
		}

		if (currentInstructionList[1].rt == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				currentInstructionList[1].currentForward.rtDelayedForward = true;
				currentInstructionList[1].currentForward.rtDelayForwardDepth = 1;
			} else {
				currentInstructionList[1].currentForward.rtDelayedForward = false;
			}
		}
	}
}
