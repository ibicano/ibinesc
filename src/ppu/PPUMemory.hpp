/*
 * PPUMemory.hpp
 *
 *  Created on: 10 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_PPUMEMORY_HPP_
#define PPU_PPUMEMORY_HPP_

class PPU;
class Mapper;


class PPUMemory {
public:
	// Constantes
    static const int ADDR_PATTERN_0 = 0x0000;
    static const int ADDR_PATTERN_1 = 0x1000;

    static const int ADDR_NAME_0 = 0x2000;
    static const int ADDR_ATTR_0 = 0x23C0;
    static const int ADDR_NAME_1 = 0x2400;
    static const int ADDR_ATTR_1 = 0x27C0;
    static const int ADDR_NAME_2 = 0x2800;
    static const int ADDR_ATTR_2 = 0x2BC0;
    static const int ADDR_NAME_3 = 0x2C00;
    static const int ADDR_ATTR_3 = 0x2FC0;

    static const int ADDR_IMAGE_PALETTE = 0x3F00;
    static const int ADDR_SPRITE_PALETTE = 0x3F10;

	PPUMemory(PPU* ppu, Mapper* mapper);
	virtual ~PPUMemory();

	int readData(int addr);
	void writeData(int data, int addr);

private:
	PPU* ppu;
	Mapper* mapper;

	// Almacena los datos de la memoria
	int* memory;

	// Escribe un dato en una posición de memoria y en todos sus mirrors
	void setMemory(int data, int addr);
};

#endif /* PPU_PPUMEMORY_HPP_ */
