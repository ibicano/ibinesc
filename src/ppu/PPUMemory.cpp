/*
 * PPUMemory.cpp
 *
 *  Created on: 10 de jul. de 2015
 *      Author: ibon
 */

//include "PPUMemory.hpp"


PPUMemory::PPUMemory(PPU* ppu, Mapper* maper) {
	this->ppu = ppu;
	this->mapper = mapper;

	memory = new int[0x10000];
}//PPUMemory()


PPUMemory::~PPUMemory() {
} //~PPUMemory


int PPUMemory::readData(int addr) {
	int a = addr & 0xFFFF;
	int d = 0x00;

	if (a < 0x2000)
		d = mapper->readChr(a);
	else if (0x2000 <= a < 0x4000)
		d = memory[a];
	else
		readData(a & 0x3FFF);

	return d;
}//readData()

void PPUMemory::writeData(int data, int addr) {
	int a = addr & 0xFFFF;
	int d = data & 0xFF;

	// Pattern tables:
	if ((0x0000 <= a <= 0x1FFF) && mapper->getChrCount() == 0) {
		mapper->writeChr(d, a);
		// reseteamos la cache de Tiles por si hay un cambio de banco en el Mapper
		ppu->resetTilesCache();
	}
	// Name tables y attribute tables:
	else if (0x2000 <= a < 0x4000) {
		// Name Table 0
		if (0x2000 <= a < 0x2400) {
			if (mapper->mirrormode() == 0) {
				setmemory(d, a + 0x0400)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x1400)
			}
			elif (mapper->mirrormode() == 1) {
				setmemory(d, a + 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x1800)
			elif mapper->mirrormode() == 2:
				setmemory(d, a + 0x0400)
				setmemory(d, a + 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x1400)
				setmemory(d, a + 0x1800)

			// Escribimos en la posición indicada
			setmemory(d, a)
		}
		// Name Table 1
		elif 0x2400 <= a < 0x2800:
			if mapper->mirrormode() == 0:
				setmemory(d, a - 0x0400)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0C00)
			elif mapper->mirrormode() == 1:
				setmemory(d, a + 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				if a < 0x2700:
					setmemory(d, a + 0x1800)
			elif mapper->mirrormode() == 2:
				setmemory(d, a - 0x0400)
				setmemory(d, a + 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0C00)
				if a < 0x2700:
					setmemory(d, a + 0x1800)

			// Escribimos en la posición indicada
			setmemory(d, a)
		// Name Table 2
		elif 0x2800 <= a < 0x2C00:
			if mapper->mirrormode() == 0:
				setmemory(d, a + 0x0400)
				// Mirrors
				setmemory(d, a + 0x1000)
				if a < 0x2B00:
					setmemory(d, a + 0x1400)
			elif mapper->mirrormode() == 1:
				setmemory(d, a - 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0800)
			elif mapper->mirrormode() == 2:
				setmemory(d, a + 0x0400)
				setmemory(d, a - 0x0800)
				// Mirrors
				setmemory(d, a + 0x1000)
				if a < 0x2B00:
					setmemory(d, a + 0x1400)
				setmemory(d, a + 0x0800)

			// Escribimos en la posición indicada
			setmemory(d, a)
		// Name Table 3
		elif 0x2C00 <= a < 0x3000:
			if mapper->mirrormode() == 0:
				setmemory(d, a - 0x0400)
				// Mirrors
				if a < 0x2F00:
					setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0C00)
			elif mapper->mirrormode() == 1:
				setmemory(d, a - 0x0800)
				// Mirrors
				if a < 0x2F00:
					setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0800)
			elif mapper->mirrormode() == 2:
				setmemory(d, a - 0x0400)
				setmemory(d, a - 0x0800)
				// Mirrors
				if a < 0x2F00:
					setmemory(d, a + 0x1000)
				setmemory(d, a + 0x0C00)
				setmemory(d, a + 0x0800)

			// Escribimos en la posición indicada
			setmemory(d, a)
	}
		// Mirrors Name Tables y Attr Tables
		elif 0x3000 <= a < 0x3F00:
			writedata(d, a - 0x1000)
		// Paletas
		elif 0x3F00 <= a < 0x3F20:
			// Si se escribe en el elemento de background o su mirror se escribe el valor de background
			// en todas las paletas mod 4 (pero no al contrario)
			if a == 0x3F00 or a == 0x3F10:
				for x in xrange(0x3F00, 0x4000, 0x04):
					setmemory(d, x)
			// Si no es un elemento de background escribimos normalmente la paleta
			elif a & 0x03 != 0:
				// Escribe en mirrors
				setmemory(d, a + 0x0020)
				setmemory(d, a + 0x0040)
				setmemory(d, a + 0x0060)
				setmemory(d, a + 0x0080)
				setmemory(d, a + 0x00A0)
				setmemory(d, a + 0x00C0)
				setmemory(d, a + 0x00E0)

				// Escribimos en la posición indicada
				setmemory(d, a)
		// Mirrors paletas
		elif 0x3F20 <= a < 0x4000:
			writedata(d, ((a - 0x3F20) % 0x0020) + 0x3F00)

	// Mirrors generales
	elif a >= 0x4000:
		writedata(d, a & 0x3FFF)
}//writeData()


void PPUMemory::setMemory(int data, int addr) {
}
