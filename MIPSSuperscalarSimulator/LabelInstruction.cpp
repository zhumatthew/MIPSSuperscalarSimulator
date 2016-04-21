//
//  LabelInstruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <string>
#include "LabelInstruction.hpp"

LabelInstruction::LabelInstruction(string label, int lineNumber) {
//    if (!label.empty() && label.back() == ':') {
//        label.pop_back();
//    }
    
//    if (label.at(label.size() - 1) == ':') {
//        label = label.substr(0, label.size()-1);
//    }
    
    if (!label.empty() && label.back() == ':') {
        label.resize(label.size()-1);
    }
    
    this->label = label;
    labelAddress = lineNumber;
}

int LabelInstruction::getLabelAddress() {
    return labelAddress;
}

string LabelInstruction::getLabelString() {
    return label;
}
