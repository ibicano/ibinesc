/*
 * MMC1.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_MMC1_HPP_
#define MAPPERS_MMC1_HPP_

#include "Mapper.hpp"

class ROM;

class MMC1 : public Mapper {
public:
	static const int MAPPER_CODE = 1;

	MMC1(ROM* rom);
	virtual ~MMC1();

	virtual int readPrg(int addr);
	virtual void writePrg(int data, int addr);

	virtual int readChr(int addr);
	virtual void writeChr(int data, int addr);

	// Devuelve el modo de mirror activo (puede variar entre mappers)
	virtual int getMirrorMode();

private:
	 // Bancos de CHR de 4K
	unsigned int* chr0;         // 0x0000
	unsigned int* chr1;         // 0x1000

	// Bancos de PGR de 16K
	unsigned int* prg0;        // 0x8000
	unsigned int* prg1;        // 0xC000

	// Memoria RAM
	unsigned int chrRam0[4096];
	unsigned int chrRam1[4096];

	int shiftReg;

	int reg0;
	int reg1;
	int reg2;
	int reg3;

	int addr13_14;
	int counter;


	void swapBanks();
};

#endif /* MAPPERS_MMC1_HPP_ */
