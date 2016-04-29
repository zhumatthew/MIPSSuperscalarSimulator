//
//  main.cpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 4/5/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

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

	vector<SimulatedInstruction> simulatedInstructionList = vector<SimulatedInstruction>();
	vector<Instruction> originalInstructionList = sourceReader.getInstructionList();

	cout << "-------------Start Simulation-------------" << endl;
	cout << "Original number of instructions : " << originalInstructionList.size() << endl;
    cout << string(OUTPUT_WIDTH, '-') << endl;


	for (int i = 0; i < originalInstructionList.size(); i++) {
		SimulatedInstruction simInstr = SimulatedInstruction(originalInstructionList[i]);
		simulatedInstructionList.push_back(simInstr);
	}

	SimulatedInstruction endInstr = SimulatedInstruction("end");
    SimulatedInstruction nopInstr = SimulatedInstruction("nop");

	simulatedInstructionList.push_back(endInstr);
    fill_n(back_inserter(simulatedInstructionList), 3, nopInstr);

	for (int i = 0; i < simulatedInstructionList.size(); i++) {
		simulatedInstructionList[i].instructionLocation = i;
	}

	Simulator sim = Simulator(simulatedInstructionList);
	sim.process();

	return 0;
}
