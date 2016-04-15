//
//  MemoryStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef MemoryStage_hpp
#define MemoryStage_hpp

#include <stdio.h>
#include <iostream>
#include "PipelineStage.hpp"
#include "MainMemory.hpp"
#include "RegisterFile.hpp"

class MemoryStage : public PipelineStage {
    
public:
	void implement(MainMemory& mainMemory, RegisterFile& regfile);

};

#endif /* MemoryStage_hpp */
