//
//  WriteBackStage.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef WriteBackStage_hpp
#define WriteBackStage_hpp

#include <stdio.h>
#include "Stage.hpp"

using namespace std;

class WriteBackStage:Stage {
public:
	WriteBackStage();
	void implement(RegisterFile regFile, DecodeStage currentDecode, Simulator sim);
private:

};

#endif /* WriteBackStage_hpp */
