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

using namespace std;

class DecodeStage : public Stage {

public:
	DecodeStage();
    int pcSaved;
    SimulationInstruction instrucionSaved;
    bool readAfterWriteHazard;

    void implement(MainMemory mmem, RegisterFile regm, Simulator currentSimulator);
    void check(vector<SimulationInstruction> hazardList, Simulator currentSimulator);

private:

};

#endif /* DecodeStage_hpp */
