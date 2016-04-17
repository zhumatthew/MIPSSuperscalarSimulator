//
//  Forwarding.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef Forwarding_hpp
#define Forwarding_hpp

#include <stdio.h>

class Forwarding {
    
public:
	Forwarding();

    bool rsForward; // Indicates whether or not rs needs to be forwarded
    bool rtForward; // Indicates whether or not rt needs to be forwarded
    bool rsDelayedForward;
    bool rtDelayedForward; // Indicates this instruction has a data dependence with the ~second to last, or penultimate instruction~
    
    // Because there are no pipeline registers in this simulator, when the source register for forwarding is EX|MEM, this field is assigned a value of 4. This indicates that when forwarding, the data will be found in the current instruction of the write-back stage. The "SW" instruction is unique in that all other instructions only need to be forwarded at the EX stage and only have one register (rs or rt) to be forwarded. Since every cycle is processed in reverse order from WB to IF, the only possibility to be considered is forwarding from EX|MEM to EX.

    int rsForwardDepth;
    int rtForwardDepth;
    int rsDelayedForwardDepth;
    int rtDelayedForwardDepth;

};

#endif /* Forwarding_hpp */
