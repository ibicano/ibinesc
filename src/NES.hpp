/*
 * NES.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef NES_HPP_
#define NES_HPP_

#include "cpu/CPU.hpp"
#include "cpu/Instruction.hpp"
#include "ppu/PPU.hpp"
#include "ROM.hpp"
#include "Input.hpp"
#include <iostream>
#include "mappers/Mapper.hpp"
#include "Config.hpp"

using namespace std;

class NES {
public:
	NES(string fileName, Config* config);
	virtual ~NES();

	void init(string fileName);

	// Ejecuta la NES
	void run();

	// Resetea el estado de la NES
	void reset();

	// Aplica y actualiza la configuración y el estado del objeto "config"
	void refreshConfig();

private:
	Config* config = NULL;		// Configuración del emulador
	bool pause;			// Indica si la emulación está en pausa
	ROM* rom = NULL;			// Rom del juego
	Mapper* mapper = NULL;		// Mapper del cartucho
	Memory* mem = NULL;
	CPU* cpu = NULL;			// CPU de la NES
	PPU* ppu = NULL;			// PPU de la NES
	Joypad* joypad = NULL;
};

#endif /* NES_HPP_ */
