//
//  PipelineStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef PipelineStage_hpp
#define PipelineStage_hpp

#include <stdio.h>
#include <vector>
#include "SimulatedInstruction.hpp"

class PipelineStage {
public:
	PipelineStage();
	vector<SimulatedInstruction> currentInstructionList;
    // index in the instruction queue for the instruction that is being fetched at this cycle
    int programCounter;

private:

};

#endif /* PipelineStage_hpp */
