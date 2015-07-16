/*
 * NROM.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_NROM_HPP_
#define MAPPERS_NROM_HPP_

#include "Mapper.hpp"

class ROM;

class NROM : public Mapper {
public:
	static const int MAPPER_CODE = 0;

	NROM(ROM* rom);
	virtual ~NROM();

	int readPrg(int addr);
	void writePrg(int data, int addr);

	int readChr(int addr);
	void writeChr(int data, int addr);

	int mirrorMode();

private:
	unsigned int* prgRom0;
	unsigned int* prgRom1;

	unsigned int* chrRom0;
	unsigned int* chrRom1;
};

#endif /* MAPPERS_NROM_HPP_ */
