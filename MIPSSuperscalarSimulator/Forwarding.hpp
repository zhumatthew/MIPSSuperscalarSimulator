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

using namespace std;

class Forwarding {
public:
	Forwarding();

	 bool rsForward;
	 bool rtForward;
	 bool rsDelayedForward;
	 bool rtDelayedForward;

	 int rsForwardDepth;
	 int rtForwardDepth;
	 int rsDelayForwardDepth;
	 int rtDelayForwardDepth;
private:

};

#endif /* Forwarding_hpp */
