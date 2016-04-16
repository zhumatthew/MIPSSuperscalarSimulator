//
//  MainMemory.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MainMemory.hpp"

#define MEMORY_SIZE 1024

MainMemory::MainMemory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memoryData.push_back(0);
    }
}

bool MainMemory::setValue(int value, int address) {
    if (address >= 0 && address < MEMORY_SIZE) {
        memoryData[address] = value;
        return true;
    }
    return false;
}

int MainMemory::getValue(int address) const {
    return memoryData[address];
}
