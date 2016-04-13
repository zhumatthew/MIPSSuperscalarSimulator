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

	cout << "-------Start-------" << endl;
	cout << originInstructionList.size() << endl;

	for (int i = 0; i < originInstructionList.size(); i++) {
		SimulationInstruction simInstr = SimulationInstruction(originInstructionList[i]);
		simulationInstructionList[i] = simInstr;
	}

	SimulationInstruction endInstr = SimulationInstruction("end");
	simulationInstructionList[simulationInstructionList.size()] = endInstr;
	SimulationInstruction nopInstr = SimulationInstruction("nop");
	simulationInstructionList[simulationInstructionList.size()] = nopInstr;
	simulationInstructionList[simulationInstructionList.size()] = nopInstr;
	simulationInstructionList[simulationInstructionList.size()] = nopInstr;

	for (int i = 0; i < simulationInstructionList.size(); i++) {
		simulationInstructionList[i].instructionLocation = i;
	}

	Simulator sim = Simulator(simulationInstructionList);
	sim.implement();

	return 0;
}
