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

class WriteBackStage : public Stage {
    
public:
	WriteBackStage();
	int implement(RegisterFile regFile, DecodeStage currentDecode);

};

#endif /* WriteBackStage_hpp */
