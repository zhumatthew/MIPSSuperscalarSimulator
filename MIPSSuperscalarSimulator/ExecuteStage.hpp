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
#include "Stage.hpp"
#include "DecodeStage.hpp"
#include "MemoryStage.hpp"

class ExecuteStage : public Stage {

public:
	void implement(DecodeStage currentDecode, MemoryStage currentMemory, const RegisterFile& regFile, int lastStall, bool falsePrediction);
	int getSavedPC();

private:
	int tempPC;
};

#endif /* ExecuteStage_hpp */
