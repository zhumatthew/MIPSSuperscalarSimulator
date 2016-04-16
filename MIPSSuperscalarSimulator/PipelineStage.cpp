//
//  Stage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "PipelineStage.hpp"

// default (initialized) instruction is a NOP
PipelineStage::PipelineStage() : currentInstructionList(2, SimulationInstruction("nop")), programCounter(0) {}
