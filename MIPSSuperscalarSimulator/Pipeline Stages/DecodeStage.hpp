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
#include "PipelineStage.hpp"
#include "../Memory/MainMemory.hpp"
#include "../Memory/RegisterFile.hpp"

class DecodeStage : public PipelineStage {

public:
	DecodeStage();
    // SimulationInstruction instructionSaved;
    bool readAfterWriteHazard;

    void process(const RegisterFile& regFile, const vector<SimulatedInstruction>& hazardList, int lastStall);
    void check(const vector<SimulatedInstruction>& hazardList, int lastStall);

};

#endif /* DecodeStage_hpp */
