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

class MainMemory {
public:
    MainMemory();
    bool putValue(int value, int address);
    int getValue(int address) const;
    
private:
    vector<int> memoryData; // holds values in main memory
};

#endif /* MainMemory_hpp */
