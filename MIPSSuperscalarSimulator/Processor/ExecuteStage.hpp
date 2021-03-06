//
//  ExecuteStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef ExecuteStage_hpp
#define ExecuteStage_hpp

#include <stdio.h>
#include "PipelineStage.hpp"
#include "DecodeStage.hpp"
#include "MemoryStage.hpp"

class ExecuteStage : public PipelineStage {

public:
	void process(DecodeStage currentDecode, MemoryStage currentMemory, RegisterFile const& regFile, int lastStall, bool& branchMisprediction);
	int getBranchTarget();

private:
	int branchTarget;
};

#endif /* ExecuteStage_hpp */
