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
#include "SimulationInstruction.hpp"

int main(int argc, const char * argv[]) {
	SourceReader sourceReader("C:\\Users\\jm28550\\Downloads\\alu_reg_1.asm");
	sourceReader.findLabel();
	sourceReader.constructInstrList();

	vector<SimulationInstruction> simulationInstructionList = vector<SimulationInstruction>();
	vector<Instruction> originInstructionList = sourceReader.getInstructionList();

	cout << "-----------Start Simulation-----------" << endl;
	cout << "Original number of Instruction : " << originInstructionList.size() << endl;

	for (int i = 0; i < originInstructionList.size(); i++) {
		SimulationInstruction simInstr = SimulationInstruction(originInstructionList[i]);
		simulationInstructionList.push_back(simInstr);
	}

	SimulationInstruction endInstr = SimulationInstruction("end");
	simulationInstructionList.push_back(endInstr);
	SimulationInstruction nopInstr = SimulationInstruction("nop");
	simulationInstructionList.push_back(nopInstr);
	simulationInstructionList.push_back(nopInstr);
	simulationInstructionList.push_back(nopInstr);

	for (int i = 0; i < simulationInstructionList.size(); i++) {
		simulationInstructionList[i].instructionLocation = i;
	}

	Simulator sim = Simulator(simulationInstructionList);
	sim.implement();

	return 0;
}
