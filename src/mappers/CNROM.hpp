/*
 * CNROM.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_CNROM_HPP_
#define MAPPERS_CNROM_HPP_

#include "Mapper.hpp"

class ROM;

class CNROM : public Mapper {
public:
	static const int MAPPER_CODE = 3;

	CNROM(ROM* rom);
	virtual ~CNROM();

	int readPrg(int addr);
	int readChr(int addr);

	void writePrg(int data, int addr);
	void writeChr(int data, int addr);

	int getMirrorMode();

private:
	unsigned int* prgRom0;
	unsigned int* prgRom1;

	unsigned int* chrRom;;
};

#endif /* MAPPERS_CNROM_HPP_ */
