//
//  main.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/5/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#include <iostream>
#include <limits>
#include "Assembler/SourceReader.hpp"
#include "Processor/SimulatedInstruction.hpp"
#include "Simulator.hpp"
#include "BufferToggle.hpp"

#define OUTPUT_WIDTH 42

int main(int argc, const char * argv[]) {

	if (argc < 2) {
		cout << "Usage: " << argv[0] << " assembly_file_name.asm" << endl;
		return 0;
	}
    
	SourceReader sourceReader(argv[1]);
	sourceReader.findLabelAddresses();
	sourceReader.constructInstructionList();

	vector<Instruction> originalInstructionList = sourceReader.getInstructionList();
    vector<SimulatedInstruction> simulatedInstructionList = vector<SimulatedInstruction>();

	cout << "-------------Start Simulation-------------" << endl;
	cout << "Original number of instructions : " << originalInstructionList.size() << endl;
    cout << string(OUTPUT_WIDTH, '-') << endl;


    for (Instruction instruction: originalInstructionList) {
		simulatedInstructionList.push_back(SimulatedInstruction(instruction));
	}

    // Inserts an end instruction followed by 3 'NOPs' to terminate the instruction list
    // An end NOP instruction is used to indicate the end of the instruction list
    
    SimulatedInstruction end = SimulatedInstruction();
    end.originalString = "end";
    end.opcodeString = "end";
	simulatedInstructionList.push_back(end);
    fill_n(back_inserter(simulatedInstructionList), 3, SimulatedInstruction());

	for (int i = 0; i < simulatedInstructionList.size(); i++) {
		simulatedInstructionList[i].instructionLocation = i;
	}

	Simulator simulator(simulatedInstructionList);
    
    cout << "Press enter to run the entire simulation without pauses" << endl;
    cout << "Or press space to step through the simulation" << endl;
    
    BufferToggle bt;
    
    while (simulator.memoryStage.currentInstructionList.front().originalString != "end") {
        
        bt.off();
        char input = getchar();
        bt.on();

        if (input == ' ') {
            cout << endl;
            simulator.stepProcess();
        } else if (input == '\n' || input == '\r') {
            simulator.process();
        }
    }
    
    
    simulator.printFinalOutput();
    
	return 0;
}
