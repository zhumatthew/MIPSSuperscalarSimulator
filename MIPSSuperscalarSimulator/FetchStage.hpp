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
#include "Stage.hpp"

using namespace std;

class FetchStage : public Stage {

public:
	FetchStage(int instructionLength);
    void implement(vector<SimulationInstruction> simulationInstructionList, int lastStall, bool falsePrediction, int savedPC);
    
private:
    int upBranch;
    int lastPC;
    int tempCnt;
    bool pairwise;
    int windowTail;
    int instrSize;
    int windowSize;
    vector<SimulationInstruction> window;

    // window/check/count?
	void windowMove(vector<SimulationInstruction> simulationInstructionList);
	bool regNameMatch(int check);
	void reorder(vector<SimulationInstruction> simuInstrList);
	void clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2);
};

#endif /* FetchStage_hpp */
