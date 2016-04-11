//
//  DecodeStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef DecodeStage_hpp
#define DecodeStage_hpp

#include <stdio.h>
#include "Stage.hpp"
#include "MainMemory.hpp"
#include "RegisterFile.hpp"

using namespace std;

class DecodeStage : public Stage {

public:
	DecodeStage();
    int pcSaved;
    // SimulationInstruction instructionSaved;
    bool readAfterWriteHazard;

    void implement(MainMemory mmem, RegisterFile regm, vector<SimulationInstruction> hazardList, int lastStall);
    void check(vector<SimulationInstruction> hazardList, int lastStall);

private:

};

#endif /* DecodeStage_hpp */
