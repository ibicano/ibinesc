/*
 * PPU.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "PPU.hpp"


PPU::PPU(Mapper* mapper) {
	memory = new PPUMemory(mapper);
	spriteMemory = new SpriteMemory();
	this->mapper = mapper;
	gfx = new GFX_Sdl2();
}

PPU::~PPU() {
	// TODO Auto-generated destructor stub
}

bool PPU::getIntVblank() {
	return false;
}

void PPU::execCycles(int cycles) {
}

int PPU::readReg(int addr) {
}

void PPU::writeReg(int data, int addr) {
}

void PPU::setIntVblank(bool v) {
}

int PPU::readReg2002() {
}

int PPU::readReg2004() {
}

int PPU::readReg2007() {
}

void PPU::writeReg2000() {
}

void PPU::writeReg2001() {
}

void PPU::writeReg2003() {
}

void PPU::writeReg2004() {
}

void PPU::writeReg2005() {
}

void PPU::writeReg2006() {
}

void PPU::writeReg2007() {
}

void PPU::writeSpriteDma(Memory* cpuMem, int srcAddr) {
}

void PPU::incrXScroll() {
}

void PPU::incrYScroll() {
}

int PPU::getXOffset() {
}

int PPU::getYOffset() {
}

int PPU::control1NametableBits0_1() {
}

int PPU::control1IncrementBit2() {
}

int PPU::control1SpritesPatternBit3() {
}

int PPU::control1BackgroundPatternBit4() {
}

int PPU::control1SpritesSizeBit5() {
}

int PPU::control1MasterModeBit6() {
}

int PPU::control1NmiBit7() {
}

int PPU::control2MonochromeBit0() {
}

int PPU::control2ClipbackgroundBit1() {
}

int PPU::control2ClipspritesBit2() {
}

int PPU::control2BackgroundBit3() {
}

int PPU::control2SpritesBit4() {
}

int PPU::control2ColourconfigBits5_7() {
}

int PPU::activeNametableAddr() {
}

int PPU::startVblank() {
}

int PPU::endVblank() {
}

int PPU::setSpriteHit(bool v) {
}

bool PPU::isVblank() {
}

void PPU::drawScanline() {
}

void PPU::drawPixel(int x, int y) {
}

void PPU::drawSprites() {
}

void PPU::drawSpritePixel(Sprite* sprite, int sprX, int sprY) {
}

void PPU::getSpritesList() {
}

void PPU::calcSpriteHit() {
}

int PPU::calcAttrColor(int nametableAddr) {
}

void PPU::fetchPattern(int patternTable, int patternIndex, int attrColor,
		int paletteAddr, int** tileIndex, RGB** tileRGB) {
}
