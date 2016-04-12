//
//  Stage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef Stage_hpp
#define Stage_hpp

#include <stdio.h>
#include <vector>
#include "SimulationInstruction.hpp"

using namespace std;

class Stage {
public:
	Stage();
	vector<SimulationInstruction> currentInstructionList;
//    static int PC;
    
    // should be static?
    int PC;


private:

};

#endif /* Stage_hpp */
