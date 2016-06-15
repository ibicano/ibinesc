/*
 * NES.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "NES.hpp"

#include <ctime>
#include <fstream>
#include <string>

#include "mappers/NROM.hpp"
#include "mappers/MMC1.hpp"
#include "mappers/CNROM.hpp"
#include "mappers/MMC3.hpp"
#include "Memory.hpp"
#include "GFX.hpp"

#include <SDL2/SDL.h>

using namespace std;


NES::NES(string fileName, Config* config) {
	this->config = config;

	pause = false;

	init(fileName);

}//NES()


NES::~NES() {
	delete joypad;
	delete cpu;
	delete ppu;
	delete rom;
}

// Carga una ROM de un fichero
void NES::init(string fileName) {
	if (this->rom != NULL) delete rom;

	rom = new ROM(fileName);

	if (this->mapper != NULL) delete mapper;

	if (rom->getMapperCode() == 0) mapper = new NROM(rom);
	else if (rom->getMapperCode() == 1) mapper = new MMC1(rom);
	else if (rom->getMapperCode() == 3) mapper = new CNROM(rom);
	else if (rom->getMapperCode() == 4) mapper = new MMC3(rom);

	if (this->ppu != NULL) delete ppu;
	ppu = new PPU(mapper);

	if (this->joypad != NULL) delete joypad;
	joypad = new Joypad();

	if (this->mem != NULL) delete mem;
	mem = new Memory(ppu, mapper, joypad);

	if (this->cpu != NULL) delete cpu;
	cpu = new CPU(mem, ppu);

	if (rom->getMapperCode() == 4)
		((MMC3*)mapper)->setCpu(cpu);

	reset();
}


void NES::run() {
	// Esto para la depuración de las instrucciones
	//fstream fichLog("/home/ibon/tmp/ibitest.log", fstream::out);

	int statsCycles = 0;		// Contador de ciclos de CPU para estadísticas de rendimiento
	unsigned int statsTotalTime = SDL_GetTicks();     // Tiempo de ejecución transcurrido para fines estadísticos
	int totalCycles = 0;		// Número de ciclos de CPU totales desde que arranca el emulador
	int cycles = 0;				// Ciclo de CPU en una iteración del bucle

	// Procesa la entrada
	int keyCounter = 0;		// Contador de ciclos hasta el siguiente chequeo de pulsación del joypad
	SDL_Event event;

	// Instrucción procesada
	Instruction* inst;
	int totalInstCount = 0;

	// Variables para emular la velocidad real de la NES por iteración del bucle
	unsigned int emuFrameTime = SDL_GetTicks();
	bool endFrame;

	// Bucle principal
	while (1) {
		if (!pause) {
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

			/*
			// Escribimos en el log de instrucciones
			int opLow = inst->getOperand() & 0xFF;
			int opHigh = (inst->getOperand() >> 8) & 0xFF;

			fichLog << hex << cpu->getRegPc();
			fichLog << "\t";
			fichLog << hex << inst->getOpcode();
			fichLog << " ";
			fichLog << hex << opLow;
			fichLog << " ";
			fichLog << hex << opHigh;
			fichLog << "\t\t";
			fichLog << hex << cpu->getRegA();
			fichLog << " ";
			fichLog << hex << cpu->getRegX();
			fichLog << " ";
			fichLog << hex << cpu->getRegY();
			fichLog << " ";
			fichLog << hex << cpu->getRegP();
			fichLog << " ";
			fichLog << hex << cpu->getRegSp();
			fichLog << "\n";

			fichLog.flush();
			*/


			cycles += inst->execute();
			totalInstCount += 1;

			// Restamos los ciclos de ejecución a la PPU
			endFrame = ppu->execCycles(cycles);

			// Aquí se detectan las pulsaciones en los dispositivos de entrada-> Por cuestiones de rendimiento, ya que
			// es bastante caro comprobar en cada iteración del bucle, se hace solo cada 10000 ciclos de CPU

			keyCounter += cycles;
			if (keyCounter > 10000) {
				// Eventos SDL (entrada por ejemplo)
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYDOWN) {
						if (event.key.keysym.sym  == SDLK_w)
							joypad->setUp(1);
						else if (event.key.keysym.sym == SDLK_s)
							joypad->setDown(1);
						else if (event.key.keysym.sym == SDLK_a)
							joypad->setLeft(1);
						else if (event.key.keysym.sym == SDLK_d)
							joypad->setRight(1);
						else if (event.key.keysym.sym == SDLK_o)
							joypad->setB(1);
						else if (event.key.keysym.sym == SDLK_p)
							joypad->setA(1);
						else if (event.key.keysym.sym == SDLK_RETURN)
							joypad->setStart(1);
						else if (event.key.keysym.sym == SDLK_RSHIFT)
							joypad->setSelect(1);
						else if (event.key.keysym.sym == SDLK_ESCAPE)
							config->setFullscreen(false);
							//ppu->getGfx()->setFullscreen(false);
					}
					else if (event.type == SDL_KEYUP) {
						if (event.key.keysym.sym == SDLK_w)
							joypad->setUp(0);
						else if (event.key.keysym.sym == SDLK_s)
							joypad->setDown(0);
						else if (event.key.keysym.sym == SDLK_a)
							joypad->setLeft(0);
						else if (event.key.keysym.sym == SDLK_d)
							joypad->setRight(0);
						else if (event.key.keysym.sym == SDLK_o)
							joypad->setB(0);
						else if (event.key.keysym.sym == SDLK_p)
							joypad->setA(0);
						else if (event.key.keysym.sym == SDLK_RETURN)
							joypad->setStart(0);
						else if (event.key.keysym.sym == SDLK_RSHIFT)
							joypad->setSelect(0);
					}
					else if (event.type == SDL_WINDOWEVENT) {
						if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
							exit(0);
						}
					}
				}//while

				keyCounter = 0;
			}

			totalCycles += cycles;              // Incrementamos el contador de ciclos totales


			// Estadísticas
			statsCycles += cycles;

			// Hacemos la media de ciclos ejecutados por segundo para fines estadísticos
			if (statsCycles > 20000) {
				unsigned int statsClock = SDL_GetTicks();

				if (statsClock - statsTotalTime >= 1000) {
					cout << statsCycles << " ciclos por segundo\n";
					statsCycles = 0;
					statsTotalTime = statsClock;
				}
			}


			// Ajusta a la velocidad real de la NES
			if (endFrame) {
				int delay= PPU::FRAME_PERIOD - (SDL_GetTicks() - emuFrameTime);
				if (delay > 0) {
					// Esperamos el tiempo equivalente a la resta del período de un frame en la NES real
					// menos el tiempo que ha tardado en dibujar ese frame el emulador
					SDL_Delay(delay);
				}
				emuFrameTime = SDL_GetTicks();
			}


			// Resetea el contador de ciclos de la iteración
			cycles = 0;
		}// if (!pause)


		// TODO: modificar esto para que no se compruebe en cada iteración del bucle principal, sino cada 500ms por ejemplo
		// y así no consuma tanto tiempo de CPU (aunque bien pensado como sólo comprueba si está actualizada la configuración
		// ya el mero hecho de comprobar si han pasado los 500ms sería igual de costoso, por lo que tal vez no sea una buena idea)
		if (config->getUpdated()) {
			refreshConfig();				// Aplicamos la configuración del objeto config
			config->setUpdated(false);		// Una vez aplicada la configuración indicamos que ya no está actualizada
		}

	}// while
	//fichLog.close();
}//run()


// TODO: darle una vuelta para ver si también se pueden resetear los contadores del bucle principal.
void NES::reset() {
	cpu->reset();
}


void NES::refreshConfig() {
	if (config->getUpdatedRomFile()) {
		init(config->getRomFile());
		config->setUpdatedRomFile(false);
	}

	if (config->getReset()) {
		this->reset();
		config->setReset(false);
	}
	pause = config->getPause();

	ppu->getGfx()->setZoomView(config->getResolution());

	ppu->getGfx()->setFullscreen(config->getFullscreen());
}
