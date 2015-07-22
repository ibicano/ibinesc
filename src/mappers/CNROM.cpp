/*
 * CNROM.cpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#include "CNROM.hpp"
#include "../ROM.hpp"

CNROM::CNROM(ROM* rom) : Mapper(rom) {
	//Carga los bancos
	prgRom0 = NULL;
	prgRom1 = NULL;

	if (rom->getPrgCount() == 1)
		prgRom0 = rom->getPrg(0);
	else if (rom->getPrgCount() == 2) {
		prgRom0 = rom->getPrg(0);
		prgRom1 = rom->getPrg(1);
	}

	chrRom = rom->getChr(0);
}//CNROM()


CNROM::~CNROM() {
}


int CNROM::readPrg(int addr) {
    int a;
	int d = 0x00;

    if (0x8000 <= addr && addr < 0xC000) {
        a = (addr - 0x8000) % 0x4000;
        d = prgRom0[a];
    }
    else if (0xC000 <= addr && addr < 0x10000) {
        a = (addr - 0x8000) % 0x4000;
        if (rom->getPrgCount() == 1)
            d = prgRom0[a];
        else if (rom->getPrgCount() == 2)
            d = prgRom1[a];
    }

    return d;
}


int CNROM::readChr(int addr) {
    int a = addr % 0x4000;
    int d = 0x00;
    if (0x0000 <= a && a < 0x2000)
        d = chrRom[a];

    return d;
}


void CNROM::writePrg(int data, int addr) {
    chrRom = rom->getChr(data & 0x03);
}


void CNROM::writeChr(int data, int addr) {}


int CNROM::mirrorMode() {
    return rom->getMirroring();
}
