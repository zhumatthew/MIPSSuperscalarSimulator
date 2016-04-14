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
#include "Stage.hpp"
#include "MainMemory.hpp"
#include "RegisterFile.hpp"

class MemoryStage : public Stage {
    
public:
	void implement(MainMemory& mmemo, RegisterFile& regfile);

};

#endif /* MemoryStage_hpp */
