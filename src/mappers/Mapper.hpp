/*
 * Mapper.h
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_MAPPER_HPP_
#define MAPPERS_MAPPER_HPP_

class ROM;

class Mapper {
public:
	Mapper(ROM* rom);
	virtual ~Mapper();

	virtual int readPrg(int addr) = 0;
	virtual void writePrg(int data, int addr) = 0;

	virtual int readChr(int addr) = 0;
	virtual void writeChr(int data, int addr) = 0;

	int getPrgCount();
	int getChrCount();

	// Se usa cada vez que se procesa un scanline
	virtual void scanlineTick();

	// Devuelve el modo de mirror activo (puede variar entre mappers)
	virtual int mirrorMode() = 0;

protected:
	ROM* rom;		// Puntero a la rom del juego
};

#endif /* MAPPERS_MAPPER_HPP_ */
