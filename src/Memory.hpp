/*
 * Memory.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

class Memory {
public:
	static const int SIZE = 0x10000;

	// Se le pasan referencias de los objetos que se mapean en memoria
	Memory(PPU* ppu, Mapper* mapper, Joypad* joypad1);

	virtual ~Memory();

	// Lee una posición de memoria
	int readData(int addr);

	// Escribe un dato en una posición de memoria
	void writeData(int data, int addr);

private:
	int memory[Memory::SIZE];	// Almacena los datos

	PPU* ppu;
	Mapper* mapper;
	Joypad* joypad1;
};

#endif /* MEMORY_HPP_ */
