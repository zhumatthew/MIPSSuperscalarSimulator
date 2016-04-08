//
//  DecodeStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef AllStage_hpp
#define AllStage_hpp

#include <stdio.h>
#include <vector>
#include "SimulationInstruction.hpp"

using namespace std;

class AllStage {
public:
	AllStage();
	vector<SimulationInstruction> currentInstrList;
private:


protected:
	static int PC;
};

#endif /* AllStage_hpp */
