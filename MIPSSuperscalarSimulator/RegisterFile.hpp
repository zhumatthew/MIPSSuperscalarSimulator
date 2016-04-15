//
//  RegisterFile.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef RegisterFile_hpp
#define RegisterFile_hpp

#include <stdio.h>
#include <vector>

class RegisterFile {
public:
    RegisterFile();
    bool putValue(int value, int address);
    int getValue(int address) const;
    
private:
    vector<int> registerData; // holds values in the register file
};

#endif /* RegisterFile_hpp */
