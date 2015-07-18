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
#include "ROM.hpp"
#include "ppu/PPU.hpp"
#include "Memory.hpp"

#include <time.h>


NES::NES(string fileName) {
	rom = new ROM(fileName);

	if (rom->getMapperCode() == 0) mapper = new NROM(rom);
	// TODO: implementar el resto de mappers
	/*
	else if (rom->getMapperCode() == 1) mapper = new MMC1(rom);
	else if (rom->getMapperCode() == 3) mapper = new CNROM(rom);
	else if (rom->getMapperCode() == 4) mapper = new MMC3(rom);
	*/

	ppu = new PPU(mapper);
	joypad = new Joypad();
	mem = new Memory(ppu, mapper, joypad);
	cpu = new CPU(mem, ppu);
}//NES()


NES::~NES() {
	delete joypad;
	delete cpu;
	delete ppu;
	delete rom;
}


void NES::run() {
	int statsCycles = 0;		// Contador de ciclos de CPU para esyadísticas de rendimiento
	time_t statsTotalTime = time(NULL);     // Tiempo de ejecución transcurrido para fines estadísticos
	int totalCycles = 0;		// Número de ciclos de CPU totales desde que arranca el emulador
	//int keyCounter = 0;		// Contador de ciclos hasta el siguiente chequeo de pulsación del joypad
	int cycles = 0;				// Ciclo de CPU en una iteración del bucle

	// Instrucción procesada
	Instruction* inst;

	// Bucle principal
	while (1) {
		// Si hay interrupciones y la CPU no está ocupada, las lanzamos
		if (ppu->getIntVblank()) {
			cpu->interruptVblank()  ;      // Procesamos VBLANK
			cycles += CPU::INT_LATENCY;
		}

		if ((!cpu->getReg_p_i_bit()) & cpu->getIrq())
			cpu->interruptIrq();

		// Fetch y Exec siguiente instrucción (si hemos ejecutado una
		// interrupción en el paso anterior será su rutina de interrupción)
		inst = cpu->fetchInst();
		cycles += inst->execute();

		// Restamos los ciclos de ejecución a la PPU
		ppu->execCycles(cycles);

		// Aquí se detectan las pulsaciones en los dispositivos de entrada-> Por cuestiones de rendimiento, ya que
		// es bastante caro comprobar en cada iteración del bucle, se hace solo cada 10000 ciclos de CPU
		/*
		keycounter += cycles;
		if (keycounter > 10000) {
			// Eventos SDL (entrada por ejemplo)
			sdlevents = sdl2->ext->getevents()
			for e in sdlevents:
				if e->type == sdl2->SDLKEYDOWN:
					if e->key->keysym->sym == sdl2->SDLKw:
						joypad1->setup(1)
					elif e->key->keysym->sym == sdl2->SDLKs:
						joypad1->setdown(1)
					elif e->key->keysym->sym == sdl2->SDLKa:
						joypad1->setleft(1)
					elif e->key->keysym->sym == sdl2->SDLKd:
						joypad1->setright(1)
					elif e->key->keysym->sym == sdl2->SDLKo:
						joypad1->setb(1)
					elif e->key->keysym->sym == sdl2->SDLKp:
						joypad1->seta(1)
					elif e->key->keysym->sym == sdl2->SDLKRETURN:
						joypad1->setstart(1)
					elif e->key->keysym->sym == sdl2->SDLKRSHIFT:
						joypad1->setselect(1)
				elif e->type == sdl2->SDLKEYUP:
					if e->key->keysym->sym == sdl2->SDLKw:
						joypad1->setup(0)
					elif e->key->keysym->sym == sdl2->SDLKs:
						joypad1->setdown(0)
					elif e->key->keysym->sym == sdl2->SDLKa:
						joypad1->setleft(0)
					elif e->key->keysym->sym == sdl2->SDLKd:
						joypad1->setright(0)
					elif e->key->keysym->sym == sdl2->SDLKo:
						joypad1->setb(0)
					elif e->key->keysym->sym == sdl2->SDLKp:
						joypad1->seta(0)
					elif e->key->keysym->sym == sdl2->SDLKRETURN:
						joypad1->setstart(0)
					elif e->key->keysym->sym == sdl2->SDLKRSHIFT:
						joypad1->setselect(0)

			keycounter = 0
		}*/

		totalCycles += cycles;              // Incrementamos el contador de ciclos totales

		// Estadísticas
		statsCycles += cycles;

		// Hacemos la media de ciclos ejecutados por segundo para fines estadísticos
		if (statsCycles > 20000) {
			time_t statsClock = time(NULL);

			if (statsClock - statsTotalTime >= 1) {
				cout << statsCycles << " ciclos por segundo\n";
				statsCycles = 0;
				statsTotalTime = statsClock;
			}
		}


		// Emula la velocidad de la NES
		//time->sleep(0->0000006)

		cycles = 0;

	}// while
}//run()
