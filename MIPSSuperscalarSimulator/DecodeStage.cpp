//
//  DecodeStage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "DecodeStage.hpp"

DecodeStage::DecodeStage() {
//super();
	this->readAfterWriteHazard = false;
}

void DecodeStage::implement(MainMemory mmem, RegisterFile regm, Simulator currentSimulator) {
	if (readAfterWriteHazard || (this->currentInstructionList[0].opcodeString == "NOP")) {
		return;
	}

	this->check(currentSimulator.hazardList, currentSimulator);
	this->currentInstructionList[0].rsValue = regm.getValue(this->currentInstructionList[0].rs);
	this->currentInstructionList[0].rtValue = regm.getValue(this->currentInstructionList[0].rt);
	this->currentInstructionList[1].rsValue = regm.getValue(this->currentInstructionList[1].rs);
	this->currentInstructionList[1].rtValue = regm.getValue(this->currentInstructionList[1].rt);
}

void DecodeStage::check(vector<SimulationInstruction> hazardList, Simulator currentSimulator) {
	if (currentSimulator.lastStall == 2) {
		if (this->currentInstructionList[0].rs == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[0].currentForward->rsForward = true;
				this->currentInstructionList[0].currentForward->rsForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[0].rt == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[0].currentForward->rtForward = true;
				this->currentInstructionList[0].currentForward->rtForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[0].rs == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[0].currentForward->rsForward = true;
				this->currentInstructionList[0].currentForward->rsForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rsForward = false;
			}
		}

		if(this->currentInstructionList[0].rt == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[0].currentForward->rtForward = true;
				this->currentInstructionList[0].currentForward->rtForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rtForward = false;
			}
		}
	} else {
		if (this->currentInstructionList[0].rs == hazardList[2].rd) {
			if(this->currentInstructionList[0].loopCount > hazardList[2].loopCount) {
				this->currentInstructionList[0].currentForward->rsForward = true;
				this->currentInstructionList[0].currentForward->rsForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[0].rt == hazardList[2].rd) {
			if(this->currentInstructionList[0].loopCount > hazardList[2].loopCount) {
				this->currentInstructionList[0].currentForward->rtForward = true;
				this->currentInstructionList[0].currentForward->rtForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[0].rs == hazardList[3].rd) {
			if(currentInstructionList[0].loopCount > hazardList[3].loopCount) {
				this->currentInstructionList[0].currentForward->rsForward = true;
				this->currentInstructionList[0].currentForward->rsForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[0].rt == hazardList[3].rd) {
			if(currentInstructionList[0].loopCount > hazardList[3].loopCount) {
				this->currentInstructionList[0].currentForward->rtForward = true;
				this->currentInstructionList[0].currentForward->rtForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[0].currentForward->rsForward || this->currentInstructionList[0].currentForward->rtForward) {

			if(this->currentInstructionList[0].currentForward->rsForward) {
				int index = this->currentInstructionList[0].currentForward->rsForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (this->currentInstructionList[0].opcodeString == "SW") {
						if (this->currentInstructionList[0].currentForward->rsForward == true) {
							readAfterWriteHazard = true;
						} else {
							readAfterWriteHazard = false;
						}
					} else {
						readAfterWriteHazard = true;
					}
				}
			}

			if(this->currentInstructionList[0].currentForward->rtForward) {
				int index = this->currentInstructionList[0].currentForward->rtForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (this->currentInstructionList[0].opcodeString == "SW") {
						if (this->currentInstructionList[0].currentForward->rsForward == true) {
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

		if (this->currentInstructionList[0].rs == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[0].currentForward->rsDelayedForward = true;
				this->currentInstructionList[0].currentForward->rsDelayForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rsDelayedForward = false;
			}
		}

		if (this->currentInstructionList[0].rt == hazardList[0].rd) {
			if(currentInstructionList[0].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[0].currentForward->rtDelayedForward = true;
				this->currentInstructionList[0].currentForward->rtDelayForwardDepth = 0;
			} else {
				this->currentInstructionList[0].currentForward->rtDelayedForward = false;
			}
		}

		if (this->currentInstructionList[0].rs == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[0].currentForward->rsDelayedForward = true;
				this->currentInstructionList[0].currentForward->rsDelayForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rsDelayedForward = false;
			}
		}

		if (this->currentInstructionList[0].rt == hazardList[1].rd) {
			if(currentInstructionList[0].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[0].currentForward->rtDelayedForward = true;
				this->currentInstructionList[0].currentForward->rtDelayForwardDepth = 1;
			} else {
				this->currentInstructionList[0].currentForward->rtDelayedForward = false;
			}
		}
	}

	if (currentSimulator.lastStall == 2) {

		if (this->currentInstructionList[1].rs == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[1].currentForward->rsForward = true;
				this->currentInstructionList[1].currentForward->rsForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[1].rt == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[1].currentForward->rtForward = true;
				this->currentInstructionList[1].currentForward->rtForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[1].rs == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[1].currentForward->rsForward = true;
				this->currentInstructionList[1].currentForward->rsForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rsForward = false;
			}
		}

		if(this->currentInstructionList[1].rt == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[1].currentForward->rtForward = true;
				this->currentInstructionList[1].currentForward->rtForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rtForward = false;
			}
		}

	} else {
		if (this->currentInstructionList[1].rs == hazardList[2].rd) {
			if(this->currentInstructionList[1].loopCount > hazardList[2].loopCount) {
				this->currentInstructionList[1].currentForward->rsForward = true;
				this->currentInstructionList[1].currentForward->rsForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[1].rt == hazardList[2].rd) {
			if(this->currentInstructionList[1].loopCount > hazardList[2].loopCount) {
				this->currentInstructionList[1].currentForward->rtForward = true;
				this->currentInstructionList[1].currentForward->rtForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[1].rs == hazardList[3].rd) {
			if(currentInstructionList[1].loopCount > hazardList[3].loopCount){
				this->currentInstructionList[1].currentForward->rsForward = true;
				this->currentInstructionList[1].currentForward->rsForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rsForward = false;
			}
		}

		if (this->currentInstructionList[1].rt == hazardList[3].rd) {
			if(currentInstructionList[1].loopCount > hazardList[3].loopCount) {
				this->currentInstructionList[1].currentForward->rtForward = true;
				this->currentInstructionList[1].currentForward->rtForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rtForward = false;
			}
		}

		if (this->currentInstructionList[1].currentForward->rsForward || this->currentInstructionList[1].currentForward->rtForward) {
			if(this->currentInstructionList[1].currentForward->rsForward) {
				int index = this->currentInstructionList[1].currentForward->rsForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (this->currentInstructionList[1].opcodeString == "SW") {
						if (this->currentInstructionList[1].currentForward->rsForward == true) {
							readAfterWriteHazard = true;
						} else {
							readAfterWriteHazard = false;
						}
					} else {
						readAfterWriteHazard = true;
					}
				}
			}

			if(this->currentInstructionList[1].currentForward->rtForward) {
				int index = this->currentInstructionList[1].currentForward->rtForwardDepth + 2;
				if (hazardList[index].opcodeString == "LW") {
					if (this->currentInstructionList[1].opcodeString == "SW") {
						if (this->currentInstructionList[1].currentForward->rsForward == true) {
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

		if (this->currentInstructionList[1].rs == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[1].currentForward->rsDelayedForward = true;
				this->currentInstructionList[1].currentForward->rsDelayForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rsDelayedForward = false;
			}
		}

		if (this->currentInstructionList[1].rt == hazardList[0].rd) {
			if(currentInstructionList[1].loopCount > hazardList[0].loopCount) {
				this->currentInstructionList[1].currentForward->rtDelayedForward = true;
				this->currentInstructionList[1].currentForward->rtDelayForwardDepth = 0;
			} else {
				this->currentInstructionList[1].currentForward->rtDelayedForward = false;
			}
		}

		if (this->currentInstructionList[1].rs == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[1].currentForward->rsDelayedForward = true;
				this->currentInstructionList[1].currentForward->rsDelayForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rsDelayedForward = false;
			}
		}

		if (this->currentInstructionList[1].rt == hazardList[1].rd) {
			if(currentInstructionList[1].loopCount > hazardList[1].loopCount) {
				this->currentInstructionList[1].currentForward->rtDelayedForward = true;
				this->currentInstructionList[1].currentForward->rtDelayForwardDepth = 1;
			} else {
				this->currentInstructionList[1].currentForward->rtDelayedForward = false;
			}
		}
	}
}
