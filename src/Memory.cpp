/*
 * Memory.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "Memory.hpp"

#include "Input.hpp"
#include "mappers/Mapper.hpp"
#include "ppu/PPU.hpp"


Memory::Memory(PPU* ppu, Mapper* mapper, Joypad* joypad1) {
	// Inicializa el aray de memoria
	for (int i = 0; i < Memory::SIZE; i++)
		memory[i] = 0x00;

	this->ppu = ppu;
	this->mapper = mapper;
	this->joypad1 = joypad1;
}//Memory()


Memory::~Memory() {
}//~Memory()


/*
 * Lee la posición de memoria indicada por el parámetro 'addr', que
 * tiene que se un número de 16 bits, y devuelve el contenido
 */
int Memory::readData(int addr) {
	int d = 0x00;
	if (addr >= 0x8000)      // Memoria del programa-> Lee del mapper de la ROM
		d = mapper->readPrg(addr);
	else if (addr >= 0x0000 && addr < 0x2000)      // Memoria RAM
		d = memory[addr];
	else if (addr >= 0x2000 && addr < 0x4000)      // Direcciones de los registros PPU
		d = ppu->readReg(0x2000 + (addr & 0x07));
	else if (addr >= 0x4000 && addr < 0x4020) {		// Más registros I/O
		if (addr == 0x4016) {   					// Registro del Joypad 1
			d = joypad1->readReg();
		}
		else if (addr == 0x4017) {}
	}
	else if (0x6000 <= addr && addr <= 0x7FFF)     			// Memoria de estado de la partida->
		d = memory[addr];

	return d;
}//readData()


/*
 * Escribe el dato 'data' en la posición de memoria 'addr'
 */
void Memory::writeData(int data, int addr) {
	int d = data & 0xFF;
	addr = addr & 0xFFFF;

	int n;
	if (addr >= 0x0000 && addr < 0x2000) {        // Espacio de memoria RAM
		n = addr & 0x7FF;
		memory[n] = d;
		memory[0x0800 + n] = d;
		memory[0x1000 + n] = d;
		memory[0x1800 + n] = d;
	}
	else if (addr >= 0x2000 && addr < 0x4000) {     // Direcciones de los registros PPU
		n = 0x2000 + (addr & 0x07);
		ppu->writeReg(d, n);
	}
	else if (addr >= 0x4000 && addr <= 0x401F) {    // Más registros I/O
		if (addr == 0x4014)      // Escritura de memoria de Sprites por DMA
			ppu->writeSpriteDma(this, d);
		else if (addr == 0x4016)        // Joypad 1
			joypad1->writeReg(d);
		else if (addr == 0x4017) {}        // Joypad 2
	}
	else if (0x6000 <= addr && addr <= 0x7FFF)
		memory[addr] = d;
	else if (addr >= 0x8000) {           // Memoria de programa-> Escribe al mapper->
		mapper->writePrg(d, addr);
		// reseteamos la cache de Tiles por si hay un cambio de banco en el Mapper
		ppu->resetTilesCache();
	}
}//writeData()
