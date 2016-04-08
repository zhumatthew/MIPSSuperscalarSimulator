//
//  LabelInstruction.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef LabelInstruction_hpp
#define LabelInstruction_hpp

#include <stdio.h>
#include <string>

using namespace std;

class LabelInstruction {
    
public:
    LabelInstruction(string label, int lineNumber);
    
private:
    string label;
    int labelAddress;
    
};

#endif /* LabelInstruction_hpp */
