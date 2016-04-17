//
//  FetchStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef FetchStage_hpp
#define FetchStage_hpp

#include <stdio.h>
#include "PipelineStage.hpp"

class FetchStage : public PipelineStage {

public:
	FetchStage(int instructionLength);
    void process(vector<SimulatedInstruction>& simulationInstructionList, int lastStall, bool branchMisprediction, int savedPC);
    
private:
    int upBranch;
    int windowTail; // number of instructions in the window
    int instructionListSize; // number of instructions in the benchmark
    
    // Before IF is implemented, the window contains all instructions that can potentially be reordered. After the IF is implemented, the window contains all reordered instructions. Instructions enter ID stage according to the order of instructions in the window
    vector<SimulatedInstruction> window;

    // window/check/count?
	void windowMove(vector<SimulatedInstruction>& simulatedInstructionList);
	bool registerNameMatch(int check);
	bool reorder(vector<SimulatedInstruction>& simulatedInstructionList);
	void clear_reordered(vector<SimulatedInstruction>& simulatedInstructionList, int cnt1, int cnt2);
};

#endif /* FetchStage_hpp */
