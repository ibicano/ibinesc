//============================================================================
// Name        : ibinesc.cpp
// Author      : Ibon Cano
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "NES.hpp"
#include <iostream>
#include "gui/GUI.hpp"
#include <SDL2/SDL.h>
#include "Config.hpp"

using namespace std;


int threadGui(void* data) {
	GUI* gui = new GUI(0, (char**)0, (Config*)data);
	return 0;
}//threadGui()


int main() {
	//Inicializamos el objeto de configuración para compartirlo entre todos los hilos
	Config* config = new Config();

	// Inicializamos SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateThread(threadGui, "threadGui", (void*)config);

	string fileName = "roms/Super Mario Bros. (E).nes";
    //string fileName = "roms/Donkey Kong Classics (USA, Europe).nes";
	//string fileName = "roms/Super Mario Bros 3 (E).nes";
	//string fileName = "tests/nestest.nes";
	//string fileName = "tests/instr_test-v4/rom_singles/02-implied.nes";
	//string fileName = "tests/instr_test-v4/official_only.nes";
	//string fileName = "tests/instr_test-v4/all_instrs.nes";
	NES *nes = new NES(fileName, config);

	nes->run();

	delete nes;
	delete config;

	return 0;
}//main()



