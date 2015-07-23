/*
 * SpriteMemory.cpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#include "SpriteMemory.hpp"

SpriteMemory::SpriteMemory() {
	for (int i = 0; i < SIZE; i++)
		memory[i] = 0x00;
}


SpriteMemory::~SpriteMemory() {
}


int SpriteMemory::readData(int addr) {
	return memory[addr];
}


void SpriteMemory::writeData(int data, int addr) {
	int a = addr & 0xFF;
	int d = data & 0xFF;
	memory[a] = d;
}
