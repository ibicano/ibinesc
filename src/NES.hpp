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

using namespace std;

class NES {
public:
	NES();
	virtual ~NES();

	// Ejecuta la NES
	void run();

private:
	ROM* rom;			// Rom del juego
	Mapper* mapper;		// Mapper del cartucho
	CPU* cpu;			// CPU de la NES
	PPU* ppu;			// PPU de la NES
	Joypad* joypad;
};

#endif /* NES_HPP_ */
