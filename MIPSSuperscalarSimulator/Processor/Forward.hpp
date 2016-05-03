//
//  Forward.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/6/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef Forward_hpp
#define Forward_hpp

#include <stdio.h>

class Forward {
    
public:
	Forward();

    bool rsForward; // Indicates whether or not rs needs to be forwarded
    bool rtForward; // Indicates whether or not rt needs to be forwarded
    bool rsDelayedForward;
    bool rtDelayedForward; // Indicates this instruction has a data dependence on the ~second to last, or penultimate instruction~

    int rsForwardDepth;
    int rtForwardDepth;
    int rsDelayedForwardDepth;
    int rtDelayedForwardDepth;

};

#endif /* Forward_hpp */
