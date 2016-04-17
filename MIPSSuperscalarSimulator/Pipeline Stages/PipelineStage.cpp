//
//  Stage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "PipelineStage.hpp"

// default (initialized) instruction is a NOP
PipelineStage::PipelineStage() : currentInstructionList(2, SimulatedInstruction("nop")), programCounter(0) {}
