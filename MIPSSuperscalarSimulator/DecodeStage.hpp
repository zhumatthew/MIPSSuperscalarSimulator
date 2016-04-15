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

class DecodeStage : public Stage {

public:
	DecodeStage();
    int pcSaved;
    // SimulationInstruction instructionSaved;
    bool readAfterWriteHazard;

    void implement(const MainMemory& mmem, const RegisterFile& regm, const vector<SimulationInstruction>& hazardList, int lastStall);
    void check(const vector<SimulationInstruction>& hazardList, int lastStall);

};

#endif /* DecodeStage_hpp */
