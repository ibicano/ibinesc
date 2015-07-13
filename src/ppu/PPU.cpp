/*
 * PPU.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "PPU.hpp"
#include "Sprite.hpp"


PPU::PPU(Mapper* mapper) {
	memory = new PPUMemory(mapper);
	spriteMemory = new SpriteMemory();
	this->mapper = mapper;
	gfx = new GFX_Sdl2();

	cyclesFrame = FRAME_CYCLES - 1;

	endFrame = false;
	endScanline = false;

	newPattern = true;

	scanlineNumber = 0;
	scanlinesPending = 0;

	for (int i = 0; i< 64; i++)
		spritesList[i] = new Sprite(this);

	spriteZero = spritesList[0];

	spriteHit = false;

	intVblank = false;
	startedVblank = false;

	vramBuffer = 0x00;

	// Registros
	regControl1 = 0x00;            // Dirección 0x2000 - write
	regControl2 = 0x00;            // Dirección 0x2001 - write
	regStatus = 0x00;               // Dirección 0x2002 - read
	regSprAddr = 0x00;             // Dirección 0x2003 - write
	regSprIo = 0x00;               // Dirección 0x2004 - write
	regVramTmp = 0x00;             // Dirección 0x2005 y 0x2006 - write (16-bit)
	regVramAddr = 0x00;            // Dirección 0x2006 - write (16-bit)
	regVramIo = 0x00;              // Dirección 0x2007 - read/write
	regSpriteDma = 0x00;           // Dirección 0x4014 - write

	// Registros estado
	regXOffset = 0x0;             // Scroll patrón (3-bit)
	tmpYOffset = 0x0;             // Alamcena el offset y de forma temporal para leerlo más rápido
	regVramSwitch = 0;
}//PPU()

PPU::~PPU() {
	for (int i = 0; i< 64; i++)
		delete spritesList[i];

	delete gfx;
	delete spriteMemory;
	delete memory;
}

bool PPU::getIntVblank() {
	return intVblank;;
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

void PPU::writeReg2000(int data) {
	int d = data & 0xFF;
	regControl1 = d;

	// Transfiere el valor de los bits 0-1 a los bits 10-11 del registro vram_tmp
	regVramTmp = setBit(regVramTmp, 10, d & 0x01);
	regVramTmp = setBit(regVramTmp, 11, d & 0x02);
}//writeReg2000()

void PPU::writeReg2001(int data) {
	regControl2 = data & 0xFF;
}//writeReg2001()

void PPU::writeReg2003(int data) {
	regSprAddr = data & 0xFF;
}//writeReg2003()

void PPU::writeReg2004(int data) {
	int d = data & 0xFF;
	regSprIo = d;
	spriteMemory->writeData(d, regSprAddr);
}//writeReg2004()

void PPU::writeReg2005(int data) {
}

void PPU::writeReg2006(int data) {
}

void PPU::writeReg2007(int data) {
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
