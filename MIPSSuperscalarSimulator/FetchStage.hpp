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
    void implement(vector<SimulationInstruction> simulationInstructionList, int lastStall, bool falsePrediction, int savedPC);
    
private:
    int upBranch;
    int lastPC;
    bool pairwise;
    int windowTail; // number of instructions in the window
    int instrSize; // number of instructions in the benchmark
    int windowSize; // number of instructions in the reorder window
    
    // Before IF is implemented, the window contains all instructions that can potentially be reordered. After the IF is implemented, the window contains all reordered instructions. Instructions enter ID stage according to the order of instructions in the window
    vector<SimulationInstruction> window;

    // window/check/count?
	void windowMove(vector<SimulationInstruction> simulationInstructionList);
	bool regNameMatch(int check);
	void reorder(vector<SimulationInstruction> simuInstrList);
	void clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2);
};

#endif /* FetchStage_hpp */
