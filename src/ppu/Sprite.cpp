/*
 * Sprite.cpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#include "Sprite.hpp"

#include <stddef.h>
#include "PPU.hpp"
#include "PPUMemory.hpp"
#include "SpriteMemory.hpp"
#include "Tile.hpp"


Sprite::Sprite(PPU* ppu) {
    offsetY = 0x00;
    index = 0x00;
    attributes = 0x00;
    offsetX = 0x00;
    spriteZero = false;

    this->ppu = ppu;

    tile0 = new Tile();
    tile1 = new Tile();		// Estos sólo se usan de forma temporal si los sprites son de 8x16
}//Sprite()


// Destructor
Sprite::~Sprite() {
	delete tile1;
	delete tile0;
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
        delete tile0;
        tile0 = ppu->fetchPattern(patternTable, getIndex(), getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE);
    }
    // Si los sprites son 8x16
    else {
        int patternTable = index & 0x01;
        index = index & 0xFE;

        // Obtenemos los tiles
        delete tile0;
        tile0 = ppu->fetchPattern(patternTable, index, getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE);

        delete tile1;
        tile1 = ppu->fetchPattern(patternTable, index + 1, getAttrColor(), PPUMemory::ADDR_SPRITE_PALETTE);
    }
}//loadByAddr()


void Sprite::loadByNumber(SpriteMemory* spriteMem, int number) {
	loadByAddr(spriteMem, number * 4);
}//loadByNumber()


Tile* Sprite::getTile0() {
	return tile0;
}


Tile* Sprite::getTile1() {
	return tile1;
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
    return ((x >= offsetX && x < (offsetX + 8)) && (y >= offsetY && y < (offsetY + sizeY)));
}//isIn()


bool Sprite::isInScanline(int scanline, int sizeBit) {
    int sizeY = sizeBit * 8 + 8;
    int y = scanline - 1;
    return (offsetY <= y && y < (offsetY + sizeY));
}//isInScanline()
