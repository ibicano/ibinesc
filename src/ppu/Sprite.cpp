/*
 * Sprite.cpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#include "Sprite.hpp"
#include "PPUMemory.hpp"
#include "PPU.hpp"
#include "SpriteMemory.hpp"


Sprite::Sprite(PPU* ppu) {
    offsetY = 0x00;
    index = 0x00;
    attributes = 0x00;
    offsetX = 0x00;
    spriteZero = false;

    this->ppu = ppu;

    // Estos sólo se usan de forma temporal si los srpites son de 16 bits
    tileIndex0 = new int*[8];
    tileRgb0 = new RGB*[8];

    tileIndex1 = new int*[8];
    tileRgb1 = new RGB*[8];

    for (int i; i < 8; i++) {
    	tileIndex0[i] = new int[8];
    	tileRgb0[i] = new RGB[8];
    	tileIndex1[i] = new int[8];
    	tileRgb1[i] = new RGB[8];
    }
}//Sprite()


// Destructor
Sprite::~Sprite() {
	for (int i; i < 8; i++) {
		delete tileIndex0[i];
		delete tileRgb0[i];
		delete tileIndex1[i];
		delete tileRgb1[i];
	}
	delete []tileIndex0;
	delete []tileRgb0;
	delete []tileIndex1;
	delete []tileRgb1;
}//~Sprite()


void Sprite::loadByAddr(SpriteMemory* spriteMem, int addr) {
    offsetY = spriteMem->readData(addr) + 1;
    index = spriteMem->readData(addr + 1);
    attributes = spriteMem->readData(addr + 2);
    offsetX = spriteMem->readData(addr + 3);

    if (addr == 0x00)
        spriteZero = true;
    else
        spriteZero = false;

    // TODO: hay que implementar que cuando está el flipping activado lo aplique a los tiles
    // Cargamos los tiles de la memoria de la PPU
    int sizeBit = ppu->control1SpritesSizeBit5();
    if (sizeBit == 0) {
        int patternTable = ppu->control1SpritesPatternBit3();

        // Obtenemos el tile
        ppu->fetchPattern(patternTable, getIndex(), getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE, tileIndex0, tileRgb0);
    }
    // Si los sprites son 8x16
    else {
        int patternTable = index & 0x01;
        index = index & 0xFE;

        // Obtenemos los tiles
        ppu->fetchPattern(patternTable, index, getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE, tileIndex0, tileRgb0);
        ppu->fetchPattern(patternTable, index + 1, getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE, tileIndex1, tileRgb1);
    }
}//loadByAddr()


void Sprite::loadByNumber(SpriteMemory* spriteMem, int number) {
	loadByAddr(spriteMem, number * 4);
}//loadByNumber()


int** Sprite::getTileIndex0() {
	return tileIndex0;
}


RGB** Sprite::getTileRgb0() {
	return tileRgb0;
}


int** Sprite::getTileIndex1() {
	return tileIndex1;
}


RGB** Sprite::getTileRgb1() {
	return tileRgb0;
}


int Sprite::getOffsetY() {
	return offsetY;
}


int Sprite::getIndex() {
	return index;
}


int Sprite::getAttributes() {
	return attributes;
}


int Sprite::getOffsetX() {
	return offsetX;
}


bool Sprite::getSpriteZero() {
	return spriteZero;
}


int Sprite::getAttrColor() {
	return attributes & 0x03;
}


int Sprite::getPriority() {
	return (attributes & 0x20) >> 5;
}


int Sprite::getHorizontalFlip() {
	return (attributes & 0x40) >> 6;
}


int Sprite::getVerticalFlip() {
	return (attributes & 0x80) >> 7;
}


bool Sprite::isIn(int x, int y, int sizeBit) {
    int sizeY = sizeBit * 8 + 8;
    return ((x >= offsetX & x < offsetX + 8) & (y >= offsetY & y < offsetY + sizeY));
}//isIn()


bool Sprite::isInScanline(int scanline, int sizeBit) {
    int sizeY = sizeBit * 8 + 8;
    int y = scanline - 1;
    return (offsetY <= y < (offsetY + sizeY));
}//isInScanline()
