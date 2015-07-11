/*
 * SpriteMemory.h
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_SPRITEMEMORY_HPP_
#define PPU_SPRITEMEMORY_HPP_

class SpriteMemory {
public:
	static const int SIZE = 256;

	SpriteMemory();
	virtual ~SpriteMemory();

	// Lee una posición de memoria
	int readData(int addr);
	void writeData(int data, int addr);

private:
	int memory[256];
};

#endif /* PPU_SPRITEMEMORY_HPP_ */
