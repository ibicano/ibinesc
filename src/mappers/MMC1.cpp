/*
 * MMC1.cpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#include "MMC1.hpp"
#include "../ROM.hpp"

MMC1::MMC1(ROM* rom) : Mapper(rom) {

    // Registros
    shiftReg = 0x00;

    reg0 = 0x00;
    reg1 = 0x00;
    reg2 = 0x00;
    reg3 = 0x00;

    // Contador de escritura
    addr13_14 = 0x0000;
    counter = 0;

    // Bancos de memoria

    // Bancos de CHR de 4K
    chr0 = NULL;         // 0x0000
    chr1 = NULL;         // 0x1000

    // Bancos de PRG de 16K
    prg0 = NULL;        // 0x8000
    prg1 = NULL;        // 0xC000

    // Se carga el estado inicial
    prg0 = rom->getPrg(0);
    prg1 = rom->getPrg(rom->getPrgCount() - 1);
}

MMC1::~MMC1() {
	// TODO Auto-generated destructor stub
}

int MMC1::readPrg(int addr) {
    int d = 0x00;

    if (0x8000 <= addr && addr <= 0xBFFF)
        d = prg0[addr - 0x8000];
    else if (0xC000 <= addr && addr <= 0xFFFF)
        d = prg1[addr - 0xC000];

    return d;
}

void MMC1::writePrg(int data, int addr) {
    int d = data & 0xFF;

    // Si estamos en el primer ciclo copiamos los bits 13 y 14 de la dirección al registro de dirección
    if (counter == 0)
        addr13_14 = addr & 0x6000;

    // Si el bit 7 del dato es 1 o la dirección es de otro registro se resetea
    if ((d & 0x80) || (addr & 0x6000 != addr13_14)) {
        shiftReg = 0x00;
        counter = 0;
    }
    else {
        shiftReg = shiftReg | ((d & 0x01) << 4);

        if (counter == 4) {
            if (addr13_14 == 0x0000)
                reg0 = shiftReg;
            else if (addr13_14 == 0x2000)
                reg1 = shiftReg;
            else if (addr13_14 == 0x4000)
                reg2 = shiftReg;
			else if (addr13_14 == 0x6000) {
                // Si el tamaño de página es de 16K
                if ((reg0 & 0x08) >> 3)
                    reg3 = shiftReg;
                // Si el tamaño de página es de 32k hay que desplazar un bit a la izquierda, ya que el 0 se ignora
                else
                    reg3 = shiftReg << 1;
			}
            swapBanks();

            shiftReg = 0x00;
            counter = 0;
        }
        else {
            shiftReg >>= 1;
            counter += 1;
        }
    }
}//writePrg()


int MMC1::readChr(int addr) {
    int a = addr % 0x4000;
    int d = 0x00;

    if (0x0000 <= a && a <= 0x0FFF)
        d = chr0[a];
    else if (0x1000 <= a && a <= 0x1FFF)
        d = chr1[a - 0x1000];

    return d;
}


void MMC1::writeChr(int data, int addr) {
    if (rom->getChrCount() == 0) {
        int a = addr % 0x4000;
        int d = data & 0xFF;

        if (0x0000 <= a && a <= 0x0FFF)
            chr0[a] = d;
        else if (0x1000 <= a && a <= 0x1FFF)
            chr1[a - 0x1000] = d;
    }
}


int MMC1::mirrorMode() {
	// Si el bit 1 del registro 0 es 0 se activa single mirroring (valor 2)
	if (reg0 & 0x02 == 0)
		return 2;
	// Si el bit 1 es 1 el bit 0 indica el tipo. 0: vertical; 1: horizontal
	else
		return (~reg0) & 0x01;
}

void MMC1::swapBanks() {
	// Intercambio de bancos CHR
	int chrSize = (reg0 & 0x10) >> 4;                // 0) bancos de 8k; 1) bancos de 4k
	int bankNumber0000 = reg1 & 0x0F ;              // Número de banco para cargar en 0x0000
	int bankNumber1000 = reg2 & 0x0F;               // Número de banco para cargar en 0x1000 (Si son de 4k)

	// Bancos de 8k
	if (chrSize == 0) {
		// Si tiene CHR-RAM se intercambia la RAM
		if (rom->getChrCount() == 0) {
			chr0 = chrRam0;
			chr1 = chrRam1;
		}
		// Si no se intercambia la ROM
		else {
			chr0 = &rom->getChr(bankNumber0000 >> 1)[0x0000];
			chr1 = &rom->getChr(bankNumber0000 >> 1)[0x1000];
		}
	}
	// Bancos de 4k
	else if (chrSize == 1) {
		// Si son bancos de RAM
		if (rom->getChrCount() == 0) {
			if (bankNumber0000 == 0)
				chr0 = chrRam0;
			else if (bankNumber0000 == 1)
				chr0 = chrRam1;

			if (bankNumber1000 == 0)
				chr1 = chrRam0;
			else if (bankNumber1000 == 1)
				chr1 = chrRam1;
		}
		else {
			chr0 = rom->getChr4k(bankNumber0000);
			chr1 = rom->getChr4k(bankNumber1000);
		}
	}

	// Intercambio de bancos PRG
	int prgSwap = (reg0 & 0x04) >> 2;        // Dirección que cargar (sólo para bancos de 16k); 0) 0xC000; 1) 0x8000
	int prgSize = (reg0 & 0x08) >> 3;         // 0) bancos de 32k; 1) bancos de 16k
	int bankNumber = (reg3 & 0x0F);           // Número del banco a cargar

	// Si el tamaño del banco es de 32k
	if (prgSize == 0) {
		int bankNumber16k0 = bankNumber;
		int bankNumber16k1 = bankNumber + 1;

		prg0 = rom->getPrg(bankNumber16k0);
		prg1 = rom->getPrg(bankNumber16k1);
	}
	// Si el tamaño del banco es de 16k
	else if (prgSize == 1) {
		// Se intercambia el banco 0xC000
		if (prgSwap == 0)
			prg1 = rom->getPrg(bankNumber);
		// Se intercambia el banco 0x8000
		else if (prgSwap == 0)
			prg0 = rom->getPrg(bankNumber);
	}

}
