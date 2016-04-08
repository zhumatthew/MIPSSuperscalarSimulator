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
#include "AllStage.hpp"

using namespace std;

class MemoryStage : public AllStage {
public:
	MemoryStage();
	void implement(MainMemory mmemo, RegisterFile regfile);

private:

};

#endif /* MemoryStage_hpp */
