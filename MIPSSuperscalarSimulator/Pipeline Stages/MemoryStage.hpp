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
#include "../Memory/MainMemory.hpp"
#include "../Memory/RegisterFile.hpp"

class MemoryStage : public PipelineStage {
    
public:
	void process(MainMemory& mainMemory, RegisterFile& regfile);

};

#endif /* MemoryStage_hpp */
