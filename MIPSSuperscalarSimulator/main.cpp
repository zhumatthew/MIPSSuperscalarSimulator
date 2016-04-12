//
//  main.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/5/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include "SourceReader.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    SourceReader sourceReader("load_delay_1.asm");
    sourceReader.findLabel();
    sourceReader.constructInstrList();
    return 0;
}
