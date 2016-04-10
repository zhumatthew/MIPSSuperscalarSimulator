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
#include "RegisterFile.hpp"
#include "DecodeStage.hpp"
#include "Simulator.hpp"

class WriteBackStage : public Stage {
public:
	WriteBackStage();
	void implement(RegisterFile regFile, DecodeStage currentDecode, Simulator sim);
private:

};

#endif /* WriteBackStage_hpp */
