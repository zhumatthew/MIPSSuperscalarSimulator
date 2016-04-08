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
#include "Stage.hpp"

using namespace std;

class MemoryStage : public Stage {
public:
	MemoryStage();
	void implement(MainMemory mmemo, RegisterFile regfile);

private:

};

#endif /* MemoryStage_hpp */
