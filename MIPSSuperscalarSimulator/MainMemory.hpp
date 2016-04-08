//
//  MainMemory.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef MainMemory_hpp
#define MainMemory_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class MainMemory {
public:
    MainMemory();
    bool putValue(int value, int address);
    int getValue(int address);
    
private:
    vector<int> memoryData;
};

#endif /* MainMemory_hpp */
