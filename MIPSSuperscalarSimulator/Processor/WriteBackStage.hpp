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
#include "PipelineStage.hpp"
#include "../Memory/RegisterFile.hpp"
#include "DecodeStage.hpp"

class WriteBackStage : public PipelineStage {
    
public:
	void process(RegisterFile& regFile, int& increment);

};

#endif /* WriteBackStage_hpp */
