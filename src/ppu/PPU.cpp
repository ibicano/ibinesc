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

	//TODO: Inicializar variable de cache de tiles
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
	//TODO
}


// Lee el registro indicado por su dirección en memoria
int PPU::readReg(int addr) {
	int d = 0x00;

	if (addr == 0x2002)
		d = readReg2002();
	else if (addr == 0x2004)
		d = readReg2004();
	else if (addr == 0x2007)
		d = readReg2007();

	return d;
}//readReg()


void PPU::writeReg(int data, int addr) {
	if (addr == 0x2000) writeReg2000(data);
	else if (addr == 0x2001) writeReg2001(data);
	else if (addr == 0x2003) writeReg2003(data);
	else if (addr == 0x2004) writeReg2004(data);
	else if (addr == 0x2005) writeReg2005(data);
	else if (addr == 0x2006) writeReg2006(data);
	else if (addr == 0x2007) writeReg2007(data);
}//writeReg()


void PPU::setIntVblank(bool v) {
	intVblank = v;
}//setIntVblank()


int PPU::readReg2002() {
	int r = regStatus;

	// Cuando se lee este registro se pone el flag de vblank a 0
	regStatus = setBit(r, 7, 0);

	regVramSwitch = 0;

	return r;
}//readReg2002()


int PPU::readReg2004() {
	int addr = regSprAddr;
	regSprIo = spriteMemory->readData(addr);

	return regSprIo;
}//readReg2004()


int PPU::readReg2007() {
	int addr = regVramAddr & 0x3FFF;
	int data = 0x00;

	// Si la dirección es de la paleta se devuelve el valor inmediatamente, sino se retrasa a la siguiente lectura
	if (addr >= 0x3F00) {
		data = memory->readData(regVramAddr - 0x1000);
		vramBuffer = data;
	}
	else {
		data = vramBuffer;
		vramBuffer = memory->readData(regVramAddr);
	}

	regVramIo = data;

	if (control1IncrementBit2() == 0)
		regVramAddr = (regVramAddr + 1) & 0xFFFF;
	else
		regVramAddr = (regVramAddr + 32) & 0xFFFF;

	return data;
}//readReg2007()


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
	int d = data & 0xFF;

	// Primera escritura en $2005
	if (regVramSwitch == 0) {
		regVramTmp = setBit(regVramTmp, 0, d & 0x08);
		regVramTmp = setBit(regVramTmp, 1, d & 0x10);
		regVramTmp = setBit(regVramTmp, 2, d & 0x20);
		regVramTmp = setBit(regVramTmp, 3, d & 0x40);
		regVramTmp = setBit(regVramTmp, 4, d & 0x80);

		// FIXME: según NinTech->txt-> No aparece en SKINNY->TXT
		regVramTmp = setBit(regVramTmp, 15, 0);

		regXOffset = d & 0x07;
		regVramSwitch = 1;
	}
	// Segunda escritura en $2005
	else {
		regVramTmp = setBit(regVramTmp, 5, d & 0x08);
		regVramTmp = setBit(regVramTmp, 6, d & 0x10);
		regVramTmp = setBit(regVramTmp, 7, d & 0x20);
		regVramTmp = setBit(regVramTmp, 8, d & 0x40);
		regVramTmp = setBit(regVramTmp, 9, d & 0x80);

		regVramTmp = setBit(regVramTmp, 12, d & 0x01);
		regVramTmp = setBit(regVramTmp, 13, d & 0x02);
		regVramTmp = setBit(regVramTmp, 14, d & 0x04);

		// FIXME: según NinTech->txt-> No aparece en SKINNY->TXT
		regVramTmp = setBit(regVramTmp, 15, 0);

		regVramSwitch = 0;
	}
}//writeReg2005()


void PPU::writeReg2006(int data) {
	int d = data & 0xFF;

	// Primera escritura en $2006
	if (regVramSwitch == 0) {
		regVramTmp = setBit(regVramTmp, 8, d & 0x01);
		regVramTmp = setBit(regVramTmp, 9, d & 0x02);
		regVramTmp = setBit(regVramTmp, 10, d & 0x04);
		regVramTmp = setBit(regVramTmp, 11, d & 0x08);
		regVramTmp = setBit(regVramTmp, 12, d & 0x10);
		regVramTmp = setBit(regVramTmp, 13, d & 0x20);

		regVramTmp = setBit(regVramTmp, 14, 0);
		regVramTmp = setBit(regVramTmp, 15, 0);

		regVramSwitch = 1;
	}
	// Segunda escritura en $2006
	else
	{
		regVramTmp = setBit(regVramTmp, 0, d & 0x01);
		regVramTmp = setBit(regVramTmp, 1, d & 0x02);
		regVramTmp = setBit(regVramTmp, 2, d & 0x04);
		regVramTmp = setBit(regVramTmp, 3, d & 0x08);
		regVramTmp = setBit(regVramTmp, 4, d & 0x10);
		regVramTmp = setBit(regVramTmp, 5, d & 0x20);
		regVramTmp = setBit(regVramTmp, 6, d & 0x40);
		regVramTmp = setBit(regVramTmp, 7, d & 0x80);

		// FIXME: según NinTech.txt. No aparece en SKINNY.TXT
		regVramTmp = setBit(regVramTmp, 15, 0);

		regVramAddr = regVramTmp;

		regVramSwitch = 0;
	}
}//writeReg2006()


void PPU::writeReg2007(int data) {
	int d = data & 0xFF;
	int a = regVramAddr;
	regVramIo = d;
	memory->writeData(d, a);

	if (control1IncrementBit2() == 0)
		regVramAddr = (regVramAddr + 1) & 0xFFFF;
	else
		regVramAddr = (regVramAddr + 32) & 0xFFFF;
}//write2007()


void PPU::writeSpriteDma(Memory* cpuMem, int srcAddr) {
	regSpriteDma = srcAddr;

	int a = (srcAddr & 0xFF) * 0x0100;
	int n = 0;
	int d = 0;
	while (n < 256) {
		d = cpuMem->readData(a);
		spriteMemory->writeData(d, n);
		a += 1;
		n += 1;
	}
}//writeSpriteDma()


void PPU::incrXScroll() {
	int r = regVramAddr;
	regXOffset = (regXOffset + 1) & 0x07;
	int bit10 = (r & 0x0400) >> 10;
	int bits0_4 = r & 0x001F;

	if (regXOffset == 0) {
		bits0_4 = (bits0_4 + 1) & 0x1F;

		if (bits0_4 == 0x0) bit10 = ~bit10 & 0x1;
	}

	r = (r & 0xFBE0) | (bit10 << 10) | bits0_4;

	regVramAddr = r;
}//incrXScroll()


void PPU::incrYScroll() {
	int r = regVramAddr;
	tmpYOffset = (((r & 0x7000) >> 12) + 1) & 0x07;
	int bit11 = (r & 0x0800) >> 11;
	int bits5_9 = (r & 0x03E0) >> 5;

	if (tmpYOffset == 0) {
		bits5_9 = (bits5_9 + 1) % 30;

		if (bits5_9 == 0x0) bit11 = ~bit11 & 0x1;
	}

	r = (r & 0x41F) | (tmpYOffset << 12) | (bit11 << 11) | (bits5_9 << 5);

	regVramAddr = r;
}//incrYScroll()


int PPU::getXOffset() {
	return getXOffset;
}


int PPU::getYOffset() {
	return (regVramAddr & 0x7000) >> 12;
}


int PPU::control1NametableBits0_1() {
	return regControl1 & 0x03;
}


int PPU::control1IncrementBit2() {
	return (regControl1 & 0x04) >> 2;
}

int PPU::control1SpritesPatternBit3() {
	return (regControl1 & 0x08) >> 3;
}

int PPU::control1BackgroundPatternBit4() {
	return (regControl1 & 0x10) >> 4;
}

int PPU::control1SpritesSizeBit5() {
	return (regControl1 & 0x20) >> 5;
}

int PPU::control1MasterModeBit6() {
	return (regControl1 & 0x40) >> 6;
}

int PPU::control1NmiBit7() {
	return (regControl1 & 0x80) >> 7;
}

int PPU::control2MonochromeBit0() {
	return (regControl2 & 0x01);
}

int PPU::control2ClipbackgroundBit1() {
	return (regControl2 & 0x02) >> 1;
}

int PPU::control2ClipspritesBit2() {
	return (regControl2 & 0x04) >> 2;
}

int PPU::control2BackgroundBit3() {
	return (regControl2 & 0x08) >> 3;
}

int PPU::control2SpritesBit4() {
	return (regControl2 & 0x10) >> 4;
}

int PPU::control2ColourconfigBits5_7() {
	return (regControl2 & 0xE0) >> 5;
}

int PPU::activeNametableAddr() {
	int nt = control1NametableBits0_1();

	int addr = 0;

	if (nt == 0x0)
		addr = 0x2000;
	else if (nt == 0x1)
		addr = 0x2400;
	else if (nt == 0x2)
		addr = 0x2800;
	else if (nt == 0x3)
		addr = 0x2C00;

	return addr;
}

int PPU::startVblank() {
	// Ponemos el bit 7 del registro de Status a 1, que indica que estamos en el período VBLANK
	regStatus = setBit(regStatus, 7, 1);

	// Activamos la interrupción si las NMI están activadas en el registro de control 1
	if (regControl1 & 0x80)
		intVblank = 1;

	// Indicamos que ya hemos procesado el período VBLANK
	startedVblank = 1;
}//startVblank()

int PPU::endVblank() {
	regStatus = 0;
    startedVblank = 0;
}

int PPU::setSpriteHit(bool v) {
	spriteHit = v;
    regStatus = setBit(regStatus, 6, v);
}

bool PPU::isVblank() {
	return scanlineNumber > 240;
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
