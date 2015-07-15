/*
 * NROM.cpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#include "NROM.hpp"
#include "../ROM.hpp"

NROM::NROM(ROM* rom) : Mapper(rom) {
	// Cargamos los bancos PRG
	if (rom->getPrgCount() == 1) {
		prgRom0 = rom->getPrg(0);
	}
	else if (rom->getPrgCount() == 2) {
		prgRom0 = rom->getPrg(0);
		prgRom1 = rom->getPrg(1);
	}

	// Cargamos los bancos CHR
	if (rom->getChrCount() == 1) {
		chrRom0 = rom->getChr(0);
	}
	else if (rom->getChrCount() == 2) {
		chrRom0 = rom->getChr(0);
		chrRom1 = rom->getChr(1);
	}
}//NROM()


NROM::~NROM() {
}

int NROM::readPrg(int addr) {
	int d = 0x00;
	int a = 0x0000;

	if (0x8000 <= addr < 0xC000) {
		a = (addr - 0x8000) % 0x4000;
		d = prgRom0[a];
	}
	else if (0xC000 <= addr < 0x10000) {
		a = (addr - 0x8000) % 0x4000;
		if (rom->getPrgCount() == 1)
			d = prgRom0[a];
		else if (rom->getPrgCount() == 2)
			d = prgRom1[a];
	}

	return d;
}//readPrg()


void NROM::writePrg(int data, int addr) {
}


int NROM::readChr(int addr) {
	int a = addr % 0x4000;
	int d = 0x00;
	if (0x0000 <= a < 0x2000)
		d = chrRom0[a];

	return d;
}


void NROM::writeChr(int data, int addr) {
}


int NROM::mirrorMode() {
	return rom->getMirroring();
}
