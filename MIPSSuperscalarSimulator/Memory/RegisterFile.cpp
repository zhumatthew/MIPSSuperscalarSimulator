//
//  RegisterFile.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "RegisterFile.hpp"

#define REGISTER_SIZE 32

RegisterFile::RegisterFile() : registerData(REGISTER_SIZE, 0) {}

bool RegisterFile::setValue(int value, int address) {
    if (address >= 0 && address < REGISTER_SIZE) {
        registerData[address] = value;
        return true;
    }
    return false;
}

int RegisterFile::getValue(int address) const {
    return registerData[address];
}
