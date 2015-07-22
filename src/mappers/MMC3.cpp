/*
 * MMC3.cpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#include "MMC3.hpp"
#include "../ROM.hpp"
#include "../cpu/CPU.hpp"

MMC3::MMC3(ROM* rom) : Mapper(rom) {
	// CPU
	cpu = NULL;

	prgRom0 = NULL;
	prgRom1 = NULL;
	prgRom2 = NULL;
	prgRom3 = NULL;

	// Bancos chr de 1k
	chrRom0 = rom->getChr1k(0);
	chrRom1 = rom->getChr1k(1);
	chrRom2 = rom->getChr1k(2);
	chrRom3 = rom->getChr1k(3);
	chrRom4 = rom->getChr1k(4);
	chrRom5 = rom->getChr1k(5);
	chrRom6 = rom->getChr1k(6);
	chrRom7 = rom->getChr1k(7);

	// Modo mirror
	mirrorMode = 0;

	// Estado de save RAM
	saveRam = 0;

	// Banco seleccionado
	bankSelect = 0;

	// Modo de swap
	bankMode = 0;

	// Indica si se debe hacer XOR 0x1000 cuando bankselect es 0-5
	bankInversion = 0;

	prgCount8k = rom->getPrgCount() * 2;

	// Iniciamos el banco correspondiente a las posiciones 0xE000-0xFFFF que es fijo siempre
	prgRom3 = rom->getPrg8k(prgCount8k - 1);

	// Registros de datos
	r0 = 0;
	r1 = 0;
	r2 = 0;
	r3 = 0;
	r4 = 0;
	r5 = 0;
	r6 = 0;
	r7 = 0;

	// Rregistro relacionados con las IRQ
	irqEnable = false;
	irqReloadFlag = false;       // Indica si se recarga el irqcounter con irqlatch
	irqLatch = 0;             // Valor con el que recargar el irqcounter
	irqCounter = 0;           // Contador de scanlines que va decrementándose y cuando llega a 0 lanza una IRQ
}

MMC3::~MMC3() {
}

int MMC3::getMirrorMode() {
	return mirrorMode;
}

int MMC3::readPrg(int addr) {
    int data = 0x0;
    int a = addr & 0x1FFF;

    if (0x8000 <= addr && addr <= 0x9FFF)
        data = prgRom0[a];
    else if (0xA000 <= addr && addr <= 0xBFFF)
        data = prgRom1[a];
    else if (0xC000 <= addr && addr <= 0xDFFF)
        data = prgRom2[a];
    else if (0xE000 <= addr && addr <= 0xFFFF)
        data = prgRom3[a];

    return data;
}

int MMC3::readChr(int addr) {
    int data = 0x0;
    int a = addr & 0x03FF;

    if (0x0000 <= addr && addr <= 0x03FF)
        data = chrRom0[a];
    else if (0x0400 <= addr && addr <= 0x07FF)
        data = chrRom1[a];
    else if (0x0800 <= addr && addr <= 0x0BFF)
        data = chrRom2[a];
    else if (0x0C00 <= addr && addr <= 0x0FFF)
        data = chrRom3[a];
    else if (0x1000 <= addr && addr <= 0x13FF)
        data = chrRom4[a];
    else if (0x1400 <= addr && addr <= 0x17FF)
        data = chrRom5[a];
    else if (0x1800 <= addr && addr <= 0x1BFF)
        data = chrRom6[a];
    else if (0x1C00 <= addr && addr <= 0x1FFF)
        data = chrRom7[a];

    return data;
}


void MMC3::writePrg(int data, int addr) {
    if (!(addr & 0x01) && (0x8000 <= addr && addr < 0xA000)) {
        bankSelect = data & 0x07;
        bankMode = (data & 0x40) >> 6;
        bankInversion = (data & 0x80) >> 7;
    }
    else if ((addr & 0x01) && (0x8000 <= addr && addr < 0xA000)) {
        if (bankSelect == 0)
            r0 = data;
        else if (bankSelect == 1)
            r1 = data;
        else if (bankSelect == 2)
            r2 = data;
        else if (bankSelect == 3)
            r3 = data;
        else if (bankSelect == 4)
            r4 = data;
        else if (bankSelect == 5)
            r5 = data;
        else if (bankSelect == 6)
            r6 = data;
        else if (bankSelect == 7)
            r7 = data;
    }
    else if (!(addr & 0x01) && (0xA000 <= addr && addr < 0xC000))
        mirrorMode = (~data) & 0x01;
    else if ((addr & 0x01) && (0xA000 <= addr && addr < 0xC000))
        saveRam = (data & 0xC0) >> 6;
    // IRQs
    else if (!(addr & 0x01) && (0xC000 <= addr && addr <= 0xDFFF))
        irqLatch = data;
    else if ((addr & 0x01) && (0xC000 <= addr && addr <= 0xDFFF))
        irqReloadFlag = 1;
    else if (!(addr & 0x01) && (0xE000 <= addr && addr <= 0xFFFF))
        irqEnable = 0;
    else if ((addr & 0x01) && (0xE000 <= addr && addr <= 0xFFFF))
        irqEnable = 1;

    swapBanks();
}


void MMC3::writeChr(int data, int addr) {
}


void MMC3::setCpu(CPU* cpu) {
	this->cpu = cpu;
}

void MMC3::scanlineTick() {
    if (irqCounter == 0) {
        if (irqEnable)
            cpu->setIrq(1);
        irqCounter = irqLatch;
    }
    else {
        irqCounter -= 1;
        if (irqReloadFlag) {
            irqCounter = irqLatch;
            irqReloadFlag = false;
        }
    }
}


void MMC3::swapBanks() {
	// Bancos CHR
	if (bankInversion == 0) {
		chrRom0 = rom->getChr1k(r0 & 0xFE);
		chrRom1 = rom->getChr1k(r0 | 0x01);

		chrRom2 = rom->getChr1k(r1 & 0xFE);
		chrRom3 = rom->getChr1k(r1 | 0x01);

		chrRom4 = rom->getChr1k(r2);

		chrRom5 = rom->getChr1k(r3);

		chrRom6 = rom->getChr1k(r4);

		chrRom7 = rom->getChr1k(r5);
	}
	else {
		chrRom4 = rom->getChr1k(r0 & 0xFE);
		chrRom5 = rom->getChr1k(r0 | 0x01);

		chrRom6 = rom->getChr1k(r1 & 0xFE);
		chrRom7 = rom->getChr1k(r1 | 0x01);

		chrRom0 = rom->getChr1k(r2);

		chrRom1 = rom->getChr1k(r3);

		chrRom2 = rom->getChr1k(r4);

		chrRom3 = rom->getChr1k(r5);
	}

	// Bancos PRG
	if (bankMode == 0) {
		prgRom0 = rom->getPrg8k(r6);
		prgRom2 = rom->getPrg8k(prgCount8k - 2);
	}
	else if (bankMode == 1) {
		prgRom0 = rom->getPrg8k(prgCount8k - 2);
		prgRom2 = rom->getPrg8k(r6);
	}

	prgRom1 = rom->getPrg8k(r7);
}//swapBanks()
