//
//  RegisterFile.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "RegisterFile.hpp"

#define REGISTER_SIZE 32

RegisterFile::RegisterFile() {
    for (int i = 0; i < REGISTER_SIZE; i++) {
        registerData.push_back(0);
    }
}

bool RegisterFile::putValue(int value, int address) {
    if (address >= 0 && address < REGISTER_SIZE) {
        registerData[address] = 0;
        return true;
    }
    return false;
}

int RegisterFile::getValue(int address) {
    return registerData[address];
}