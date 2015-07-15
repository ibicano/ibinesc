//============================================================================
// Name        : ibinesc.cpp
// Author      : Ibon Cano
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "NES.hpp"
#include <iostream>

using namespace std;

int main() {
	string fileName = "../roms/Super Mario Bros. (E).nes";
    //string fileName = "../roms/Donkey Kong Classics (USA, Europe).nes";
	//string fileName = "../roms/Super Mario Bros 3 (E).nes";
	//string fileName = "../tests/nestest.nes";
	//string fileName = "../tests/instr_test-v4/rom_singles/10-branches.nes";
	//string fileName = "../tests/instr_test-v4/official_only.nes";
	//string fileName = "../tests/instr_test-v4/all_instrs.nes";
	NES *nes = new NES(fileName);

	nes->run();

	return 0;
}
