//
//  Stage.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "PipelineStage.hpp"

#define THROUGHPUT 2

// default (initialized) instructions are a NOPs
PipelineStage::PipelineStage() : currentInstructionList(THROUGHPUT, SimulatedInstruction()), programCounter(0) {}
