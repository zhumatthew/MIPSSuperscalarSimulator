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
#include "Simulator.hpp"

using namespace std;

class FetchStage : Stage {

public:
	FetchStage(int instructionLength);
	void implement(vector<SimulationInstruction> simuInstrList, Simulator currentSimu);

private:

    int upBranch;
    int lastPC;
    int tempCnt;
    bool pairwise;
    int windowTail;
    int instrSize;
    int windowSize;
    vector<SimulationInstruction> window;

	void windowMove(vector<SimulationInstruction> simulationInstructionList);
	bool regNameMatch(vector<SimulationInstruction> win, int chk);
	void reorder(vector<SimulationInstruction> win, vector<SimulationInstruction> simuInstrList);
	void clear_reordered(vector<SimulationInstruction> simulationInstructionList, int cnt1, int cnt2);
};

#endif /* FetchStage_hpp */
