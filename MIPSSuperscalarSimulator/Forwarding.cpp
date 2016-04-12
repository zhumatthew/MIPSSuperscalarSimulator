//
//  Forwarding.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "Forwarding.hpp"

using namespace std;

Forwarding::Forwarding() : rsForward(false), rtForward(false), rsDelayedForward(false), rtDelayedForward(false), rsForwardDepth(0), rtForwardDepth(0), rsDelayForwardDepth(0), rtDelayForwardDepth (0) {}
