/*
 * NROM.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_NROM_HPP_
#define MAPPERS_NROM_HPP_

#include "Mapper.hpp"

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
	int* prgRom0;
	int* prgRom1;

	int* chrRom0;
	int* chrRom1;
};

#endif /* MAPPERS_NROM_HPP_ */
