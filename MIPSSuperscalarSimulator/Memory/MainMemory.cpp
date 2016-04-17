//
//  MainMemory.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "MainMemory.hpp"

#define MEMORY_SIZE 1024

// Array rather than vector?
MainMemory::MainMemory() : memoryData(MEMORY_SIZE, 0) {}

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
