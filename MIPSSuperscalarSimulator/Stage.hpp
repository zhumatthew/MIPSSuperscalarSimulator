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
    // index of instruction queue / instruction list
    // index of instruction that is being fetched at this cycle
    int PC;
    
    
    /*
     The advantage of simulating the pipeline stages "backwards" (e.g. in reverse order like writeback, cache, alu, register, decode, fetch) is         that each stage can read the variables that represent the input latches and then simply overwrite the variables that represent the          output latches. This will not work if you simulate the pipeline "forward" because each stage would overwrite the input of the following stage and the original input would be lost. Of course, if you copy the latches before you overwrite them, you can simulate the pipeline stages in any order you want. So it's rather a minor implementation detail. A simple MIPS simulator that does this reverse order trick          is SimpleScalar.
     */


private:

};

#endif /* Stage_hpp */
