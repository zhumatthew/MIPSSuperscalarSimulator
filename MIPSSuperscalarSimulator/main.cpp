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

// mem-wb_to_ex means the write output of the memory stage of an instruction that is currently in the writeback stage is forwarded to the instruction in the execution stage (2 cycles apart)
// ex-mem_to_ex means the write output of the execute stage of an instruction that is currently in the writeback stage is forwarded to the instruction in the execution stage (1 cycle apart)

// ld is for load word
// ld_add_ex-mem_ex is for a load word that uses an address dependent on a forward between instructions 1 cycle apart (execution stage instruction depends on the memory stage instruction)
// ld_add_ex-wb_ex is for a load word that uses an address dependent on a forward between instructions 2 cycles apart (execute stage instruction depends on the write back instruction)
// ld_addr_ex-wb_mem is the memory stage instruction (sw) writing a value into memory that is dependent on the instruction in the writeback stage (lw)

// st is for store word
// st_addr_ex-mem_ex effective address for sw instruction in ex is dependent on the instruction in mem
// st_addr_mem-wb_ex the effective address for sw instruction in ex is dependent on the instruction in wb
// st_mem-wb_mem, the value written to memory is when sw is in memory is dependent on the value written back by the instruction in the wb stage (add)

// load_delay is to ensure that when load word loads to a register, the next instruction does not enter into the pipeline simultaneously because it must read after the load writes.

#include <iostream>
#include <limits>
#include "SourceReader.hpp"
#include "Simulator.hpp"
#include "SimulatedInstruction.hpp"
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
    // An infinite loop can be used to simulate the end of the instruction list
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
    
    cout << "Press enter to run the entire simulation without pauses" << endl;
    cout << "Or press space to step through the simulation" << endl;
    BufferToggle bt;
    while (simulator.memoryStage.currentInstructionList.front().originalString != "end") {
        bt.off();
        char input = getchar();
        bt.on();

//        cin >> noskipws >> input;
//        cin.get(input);
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
