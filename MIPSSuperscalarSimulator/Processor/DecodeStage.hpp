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
    bool loadDataDependence;

    void process(RegisterFile const& regFile, vector<SimulatedInstruction> const& hazardList, int lastStall);
    void check(vector<SimulatedInstruction> const& hazardList, int lastStall);

};

#endif /* DecodeStage_hpp */
