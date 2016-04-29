//
//  main.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/5/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

// Todo list:
// reduce the 5 types of instructions to the 3 standard types
// Figure out the error involving the readAfterWrite flag and the rs-rs, rt-rs control flow issue
// In memory stage, determine whether it is necessary to have both lw before sw in case an sw comes before lw

#include <iostream>
#include "SourceReader.hpp"
#include "Simulator.hpp"
#include "SimulatedInstruction.hpp"

#define OUTPUT_WIDTH 42

int main(int argc, const char * argv[]) {

	if (argc < 2) {
		cout << "Usage: " << argv[0] << " assembly_file_name.asm" << endl;
		return 0;
	}
    
	SourceReader sourceReader(argv[1]);
	sourceReader.findLabel();
	sourceReader.constructInstrList();

	vector<Instruction> originalInstructionList = sourceReader.getInstructionList();
    vector<SimulatedInstruction> simulatedInstructionList = vector<SimulatedInstruction>();

	cout << "-------------Start Simulation-------------" << endl;
	cout << "Original number of instructions : " << originalInstructionList.size() << endl;
    cout << string(OUTPUT_WIDTH, '-') << endl;


    for (Instruction instruction: originalInstructionList) {
		simulatedInstructionList.push_back(SimulatedInstruction(instruction));
	}

    // Inserts an end instruction followed by 3 'NOPs' to terminate the instruction list
    // This infinite loop can be used to simulate the end of the instruction list
    // end: j end
    // Alternatively, an instruction such as "end: NOP" aka "end: sll r0, r0, 0" may indicate the end
    
    SimulatedInstruction end = SimulatedInstruction();
    end.originalString = "end";
    end.opcodeString = "end";
	simulatedInstructionList.push_back(end);
    fill_n(back_inserter(simulatedInstructionList), 3, SimulatedInstruction());

	for (int i = 0; i < simulatedInstructionList.size(); i++) {
		simulatedInstructionList[i].instructionLocation = i;
	}

	Simulator simulator(simulatedInstructionList);
	simulator.process();

	return 0;
}
