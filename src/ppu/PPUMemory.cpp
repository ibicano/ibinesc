/*
 * PPUMemory.cpp
 *
 *  Created on: 10 de jul. de 2015
 *      Author: ibon
 */

#include "PPUMemory.hpp"
#include "../mappers/Mapper.hpp"
#include "PPU.hpp"


PPUMemory::PPUMemory(PPU* ppu, Mapper* mapper) {
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
	else if (0x2000 <= a && a < 0x4000)
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
			if (mapper->mirrorMode() == 0) {
				setMemory(d, a + 0x0400);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x1400);
			}//if
			else if (mapper->mirrorMode() == 1) {
				setMemory(d, a + 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x1800);
			}//else if
			else if (mapper->mirrorMode() == 2) {
				setMemory(d, a + 0x0400);
				setMemory(d, a + 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x1400);
				setMemory(d, a + 0x1800);
			}//else if
			// Escribimos en la posición indicada
			setMemory(d, a);
		}//if
		// Name Table 1
		else if (0x2400 <= a < 0x2800) {
			if (mapper->mirrorMode() == 0) {
				setMemory(d, a - 0x0400);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0C00);
			}
			else if (mapper->mirrorMode() == 1) {
				setMemory(d, a + 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				if (a < 0x2700)
					setMemory(d, a + 0x1800);
			}
			else if (mapper->mirrorMode() == 2) {
				setMemory(d, a - 0x0400);
				setMemory(d, a + 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0C00);
				if (a < 0x2700)
					setMemory(d, a + 0x1800);
			}
			// Escribimos en la posición indicada
			setMemory(d, a);
		}//else if
		// Name Table 2
		else if (0x2800 <= a < 0x2C00) {
			if (mapper->mirrorMode() == 0) {
				setMemory(d, a + 0x0400);
				// Mirrors
				setMemory(d, a + 0x1000);
				if (a < 0x2B00)
					setMemory(d, a + 0x1400);
			}
			else if (mapper->mirrorMode() == 1) {
				setMemory(d, a - 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0800);
			}
			else if (mapper->mirrorMode() == 2) {
				setMemory(d, a + 0x0400);
				setMemory(d, a - 0x0800);
				// Mirrors
				setMemory(d, a + 0x1000);
				if (a < 0x2B00)
					setMemory(d, a + 0x1400);
				setMemory(d, a + 0x0800);
			}
			// Escribimos en la posición indicada
			setMemory(d, a);
		}//else if
		// Name Table 3
		else if (0x2C00 <= a < 0x3000) {
			if (mapper->mirrorMode() == 0) {
				setMemory(d, a - 0x0400);
				// Mirrors
				if (a < 0x2F00)
					setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0C00);
			}
			else if (mapper->mirrorMode() == 1) {
				setMemory(d, a - 0x0800);
				// Mirrors
				if (a < 0x2F00)
					setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0800);
			}
			else if (mapper->mirrorMode() == 2) {
				setMemory(d, a - 0x0400);
				setMemory(d, a - 0x0800);
				// Mirrors
				if (a < 0x2F00)
					setMemory(d, a + 0x1000);
				setMemory(d, a + 0x0C00);
				setMemory(d, a + 0x0800);
			}
			// Escribimos en la posición indicada
			setMemory(d, a);
		}
		// Mirrors Name Tables y Attr Tables
		else if (0x3000 <= a < 0x3F00) {
			writeData(d, a - 0x1000);
		}
		// Paletas
		else if (0x3F00 <= a < 0x3F20) {
			// Si se escribe en el elemento de background o su mirror se escribe el valor de background
			// en todas las paletas mod 4 (pero no al contrario)
			if (a == 0x3F00 or a == 0x3F10) {
				for (int x = 0x3F00; x < 0x4000; x = x + 0x04)
					setMemory(d, x);
			}
			// Si no es un elemento de background escribimos normalmente la paleta
			else if (a & 0x03 != 0) {
				// Escribe en mirrors
				setMemory(d, a + 0x0020);
				setMemory(d, a + 0x0040);
				setMemory(d, a + 0x0060);
				setMemory(d, a + 0x0080);
				setMemory(d, a + 0x00A0);
				setMemory(d, a + 0x00C0);
				setMemory(d, a + 0x00E0);

				// Escribimos en la posición indicada
				setMemory(d, a);
			}//else if
		}
		// Mirrors paletas
		else if (0x3F20 <= a < 0x4000)
			writeData(d, ((a - 0x3F20) % 0x0020) + 0x3F00);
	}
	// Mirrors generales
	else if (a >= 0x4000)
		writeData(d, a & 0x3FFF);
}//writeData()


void PPUMemory::setMemory(int data, int addr) {
    int a = addr & 0x3FFF;
    memory[a] = data;
    memory[a + 0x4000] = data;
    memory[a + 0x8000] = data;
    memory[a + 0xC000] = data;
}//setMemory()
