//
//  InstructionParser.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/7/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "InstructionParser.hpp"

using namespace std;

InstructionParser::InstructionParser(string line) : line(line) {}

InstructionParser::InstructionParser(vector<string> separatedLine, vector<LabelInstruction> labelInstructionList) : separatedLine(separatedLine), labelInstructionList(labelInstructionList) {}

bool starts_with(string const& s1, string const& s2) {
    return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
}

inline bool ends_with(string const& value, string const& ending)
{
    if (ending.size() > value.size()) return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

vector<string> split(string const& text, char sep) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != string::npos) {
        string temp = text.substr(start, end - start);
        if (!temp.empty()) tokens.push_back(temp);
        start = end + 1;
    }
    string temp = text.substr(start);
    if (!temp.empty()) tokens.push_back(temp);
    
    if (tokens.back().back() == '\r') {
        tokens.back().pop_back();
    }
    
    return tokens;
}

// Splits the line into substrings delimited by the ' ' space character
vector<string> InstructionParser::splitLine() {
    separatedLine = split(line, ' ');
    
    // Remove commas that occur at the end of substrings
    for (int i = 0; i < separatedLine.size(); i++) {
        if (ends_with(separatedLine[i], ",")) {
            separatedLine[i].pop_back();
        }
    }
    
    return separatedLine;
}

Opcode InstructionParser::parseOpcode(InstrType instrType) {
    string operation = "";
    operation = separatedLine.front();
    
    return InstructionType().operationCodeDefine(operation, instrType);
}

int InstructionParser::parseRs(InstrType instrType) {
    
    int rs = 0;
    
    switch (instrType) {
        case RType:
            if (starts_with(separatedLine[2], "$")) {
                string strValue = separatedLine[2].substr(1, separatedLine[2].length());
                rs = atoi(strValue.c_str());
            }
            break;
        case IType:
            int leftIndex,rightIndex;
            
            if (separatedLine[0] == "lw") {
                leftIndex = static_cast<int>(separatedLine[2].find("("));
                rightIndex = static_cast<int>(separatedLine[2].find(")"));
                string str = separatedLine[2].substr(leftIndex + 1, rightIndex);
                if (starts_with(str, "$")) {
                    string strValue = str.substr(1, str.length());
                    rs = atoi(strValue.c_str());
                }
                break;
            } else if (separatedLine[0] == "sw") {
                leftIndex = static_cast<int>(separatedLine[2].find("("));
                rightIndex = static_cast<int>(separatedLine[2].find(")"));
                string str = separatedLine[2].substr(leftIndex + 1, rightIndex);
                if (starts_with(str, "$")){
                    string strValue = str.substr(1,str.length());
                    rs = atoi(strValue.c_str());
                }
                break;
            } else if (separatedLine[0] == "beq" || separatedLine[0] == "blez" || separatedLine[0] == "bgtz") {
                if (starts_with(separatedLine[1], "$"))
                    rs = atoi(separatedLine[1].substr(1, separatedLine[1].length()).c_str());
                break;
            }
            
            if (starts_with(separatedLine[2], "$")) {
                string strValue = separatedLine[2].substr(1, separatedLine[2].length());
                rs = atoi(strValue.c_str());
            }
            break;
        case JType:
        default:
            break;
    }
    return rs;
}

int InstructionParser::parseRt(InstrType instrType) {
    
    int rt = 0;
    
    switch (instrType) {
        case RType:
            if (starts_with(separatedLine[3], "$")) {
                string strValue = separatedLine[3].substr(1, separatedLine[3].length());
                rt = atoi(strValue.c_str());
            }
            break;
        case IType:
            
            if (separatedLine.front() == "lw") {
                if (starts_with(separatedLine[1], "$"))
                    rt = atoi(separatedLine[1].substr(1, separatedLine[1].length()).c_str());
                break;
            } else if (separatedLine.front() == "sw") {
                if (starts_with(separatedLine[1], "$"))
                    rt = atoi(separatedLine[1].substr(1,separatedLine[1].length()).c_str());
                break;
            } else if (separatedLine.front() == "beq") {
                if (starts_with(separatedLine[2], "$"))
                    rt = atoi(separatedLine[2].substr(1,separatedLine[2].length()).c_str());
                break;
            } else if (separatedLine.front() == "bgtz" || separatedLine.front() == "blez") {
                break;
            }
            
            if (starts_with(separatedLine[1], "$")) {
                string strValue = separatedLine[1].substr(1, separatedLine[1].length());
                rt = atoi(strValue.c_str());
            }
            
            
            break;
        case JType:
        default:
            break;
    }
    return rt;
}

int InstructionParser::parseRd(InstrType instrType) {
    int rd = 0;
    switch (instrType) {
        case RType:
            if (starts_with(separatedLine[1], "$")) {
                string strValue = separatedLine[1].substr(1, separatedLine[1].length());
                rd = atoi(strValue.c_str());
            }
            break;
        case IType:
            
            if (separatedLine[0] == "lw") {
                if (starts_with(separatedLine[1], "$"))
                    rd = atoi(separatedLine[1].substr(1, separatedLine[1].length()).c_str());
                break;
            } else if (separatedLine[0] == "sw" || separatedLine[0] == "beq" || separatedLine[0] == "bgtz" || separatedLine[0] == "blez") {
                break;
            }
            
            if (starts_with(separatedLine[1], "$")) {
                string strValue = separatedLine[1].substr(1, separatedLine[1].length());
                rd = atoi(strValue.c_str());
            }
            
            break;
        case JType:
        default:
            break;
    }
    return rd;
}

Funct InstructionParser::parseFunct(InstrType instrType) {
    Funct funct = funct_zero;
    if (instrType == RType) {
        funct = InstructionType().functDefine(separatedLine[0], instrType);
    }
    return funct;
}

// Parse shift amount
int InstructionParser::parseShamt(InstrType instrType) {
    int shamt = 0;
    if (instrType == RType) {
        
    }
    
    return shamt;
}

// Parse immediate
int InstructionParser::parseImmediate(InstrType instrType) {
    
    int immediate = 0;
    
    switch (instrType) {
        case RType:
            break;
        case IType:
            if (separatedLine.front() == "lw") {
                int index;
                index = static_cast<int>(separatedLine[2].find("("));
                string str = separatedLine[2].substr(0, index);
                immediate = atoi(str.c_str());
                break;
            } else if (separatedLine.front() == "sw") {
                int index;
                index = static_cast<int>(separatedLine[2].find("("));
                string str = separatedLine[2].substr(0, index);
                immediate = atoi(str.c_str());
                break;
            } else if (separatedLine.front() == "beq") {
                for (LabelInstruction instruction: labelInstructionList) {
                    if (instruction.getLabelString() == separatedLine[3]) {
                        immediate = instruction.getLabelAddress();
                    }
                }
                break;
            } else if (separatedLine.front() == "bgtz" || separatedLine.front() == "blez") {
                for (LabelInstruction instruction: labelInstructionList) {
                    if (instruction.getLabelString() == separatedLine[2]) {
                        immediate = instruction.getLabelAddress();
                    }
                }
                break;
            }
            immediate = atoi(separatedLine[3].c_str());
            break;
        case JType:
            for (LabelInstruction instruction: labelInstructionList) {
                if (instruction.getLabelString() == separatedLine[1]) {
                    immediate = instruction.getLabelAddress();
                }
            }
            break;
        default:
            break;
    }
    return immediate;
}
