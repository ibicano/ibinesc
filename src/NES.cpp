/*
 * NES.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "NES.hpp"

#include "mappers/CNROM.hpp"
#include "mappers/MMC1.hpp"
#include "mappers/MMC3.hpp"
#include "mappers/NROM.hpp"

NES::NES() {
	rom = new ROM();
	ppu = new PPU();
	cpu = new CPU();
	joypad = new Joypad();

	if (rom->getMapperCode() == 0) mapper = new NROM();
	else if (rom->getMapperCode() == 1) mapper = new MMC1();
	else if (rom->getMapperCode() == 3) mapper = new CNROM();
	else if (rom->getMapperCode() == 4) mapper = new MMC3();
}

NES::~NES() {
	delete joypad;
	delete cpu;
	delete ppu;
	delete rom;
}

void NES::run() {
	int stats_cycles = 0;		// Contador de ciclos de CPU para esyadísticas de rendimiento
	int total_cycles = 0;		// Número de ciclos de CPU totales desde que arranca el emulador
	int key_counter = 0;		// Contador de ciclos hasta el siguiente chequeo de pulsación del joypad
	int cycles = 0;				// Ciclo de CPU en una iteración del bucle

	// Instrucción procesada
	Instruction* inst;

	// Bucle principal
	while (1) {
		if (ppu->getIntVblank()) {

		}
		// Fetch y Exec siguiente instrucción (si hemos ejecutado una
		// interrupción en el paso anterior será su rutina de interrupción)
		inst = cpu->fetchInst();
		cycles += inst->execute();


	}// while
}
