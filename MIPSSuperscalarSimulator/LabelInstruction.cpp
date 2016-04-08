//
//  LabelInstruction.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include "LabelInstruction.hpp"

LabelInstruction::LabelInstruction(string label, int lineNumber) {
    if (label.back() == ':') {
        label.pop_back();
    }
    this->label = label;
    this->labelAddress = lineNumber;
}

int LabelInstruction::getLabelAddress() {
    return labelAddress;
}

string LabelInstruction::getLabelString() {
    return label;
}