/*
 * PPU.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "PPU.hpp"
#include "Sprite.hpp"
#include "PPUMemory.hpp"
#include "SpriteMemory.hpp"
#include "../GFX.hpp"
#include "../Memory.hpp"
#include "../mappers/Mapper.hpp"
#include "../nesutils.hpp"

bool operator<(const TileCacheKey& key1, const TileCacheKey& key2) {
	return (key1.tableNumber < key2.tableNumber) || (key1.index < key2.index) || (key1.color < key2.color);
}

bool operator==(const TileCacheKey& key1, const TileCacheKey& key2) {
	return (key1.tableNumber == key2.tableNumber) && (key1.index == key2.index) && (key1.color == key2.color);
}

PPU::PPU(Mapper* mapper) {
	memory = new PPUMemory(this, mapper);
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

	//Inicializa variable de cache de tiles
	resetTilesCache();
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
	return regXOffset;
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
    int y = scanlineNumber - 1;

    if (control2BackgroundBit3()) {
        // Copia el desplazamiento X del registro tmp al addr al principio del scanline
        int tmp = regVramTmp;

        // Copia los bits correspondientes del registro temporal al de dirección al principio
        // del scanline
        regVramAddr = (regVramAddr & 0b1111101111100000) | (tmp & 0x41F);

        // Pintamos el pixel
        int x = 0;
        while (x < 256) {
            drawPixel(x, y);

            // Incrementamos el registro de dirección horizontalmente si estamos pintando el background
            incrXScroll();

            // Si hemos dibujado el último pixel en anchura del "pattern", indicamos que hay que usar otro
            if (regXOffset == 0)
                newPattern = true;

            x += 1;
        }

        // Incrementamos el registro de dirección verticalmente si estamos pintando el background
        incrYScroll();
    }//if

    // Si aún no se ha producido, calculamos si ha habido impacto del Sprite zero con el fondo
    if (!spriteHit)
        calcSpriteHit();


    // Cuando se termina de dibujar el scanline siempre hay que usar otro "pattern"
    newPattern = true;
}//drawScanline()


void PPU::drawPixel(int x, int y) {
    pixelBackground[x][y] = 0;

    // Dibuja el fondo
    int patternPixelX = regXOffset;
    int patternPixelY = tmpYOffset;

    // Calcula la dirección en la Name Table activa
    int nametableAddr = 0x2000 + (regVramAddr & 0x0FFF);

    // FIXME: hacer bien la reserva de memoria de los tiles, ya que resulta confuso
    if (newPattern) {
        int patternTableNumber = control1BackgroundPatternBit4();
        int patternIndex = memory->readData(nametableAddr);

        int attrColor = calcAttrColor(nametableAddr);

        TileCacheKey key = {patternTableNumber, patternIndex, attrColor};
        TilePair tilePair;

        if (tilesCache.count(key) > 0) {
            tilePair = tilesCache[key];
        	tileBgIndex = tilePair.tileIndex;
        	tileBgRgb = tilePair.tileRgb;
        }
        else {
            fetchPattern(patternTableNumber, patternIndex, attrColor, PPUMemory::ADDR_IMAGE_PALETTE, tileBgIndex, tileBgRgb);
            tilePair.tileIndex = tileBgIndex;
            tilePair.tileRgb = tileBgRgb;
            tilesCache[key] = tilePair;
        }

        newPattern = false;
    }

    // Desactivado el clipping por cuestiones de rendimiento
    //if control2clipbackgroundbit1() or x >= 8:

    // Comprueba si el pixel actual es de background
    if (tileBgIndex[patternPixelX][patternPixelY] & 0x03)
        pixelBackground[x][y] = 1;

    gfx->drawPixel(x, y, tileBgRgb[patternPixelX][patternPixelY]);
}


void PPU::drawSprites() {
    int sizeBit = control1SpritesSizeBit5();
    int n = 0;
    while (n < 64) {
        Sprite* sprite = spritesList[n];

        int sizeY = sizeBit * 8 + 8;

        for (int sprX = 0; sprX < 8; sprX++) {
            for (int sprY = 0; sprY < sizeY; sprY++)
                drawSpritePixel(sprite, sprX, sprY);
        }

        n += 1;
    }//while
}//drawSprites()


// Dibuja el pixel (spr_x, spr_y) del sprite en pantalla en su posición correspondiente
void PPU::drawSpritePixel(Sprite* sprite, int sprX, int sprY) {
	// Tamaño de los sprites
	int sizeBit = control1SpritesSizeBit5();

	int offX = sprite->getOffsetX();
	int offY = sprite->getOffsetY();

	int screenX = offX + sprX;
	int screenY = offY + sprY;

	if (screenX < 256 && screenY < 240) {
		int pixelBck = pixelBackground[screenX][screenY];
		// Si los sprites son 8x8
		if (sizeBit == 0) {
			// Obtenemos el tile
			tileSpriteIndex = sprite->getTileIndex0();
			tileSpriteRgb = sprite->getTileRgb0();

			// Si está activado el flag de invertir horizontalmente
			if (sprite->getHorizontalFlip())
				sprX = 7 - sprX;

			// Si está activado el flag de invertir verticalmente
			if (sprite->getVerticalFlip())
				sprY = 7 - sprY;
		}
		// Si los sprites son 8x16
		else {
			bool verticalFlip = sprite->getVerticalFlip();

			// Obtenemos los tiles
			if (sprY < 8) {
				if (verticalFlip == 0) {
					tileSpriteIndex = sprite->getTileIndex0();
					tileSpriteRgb = sprite->getTileRgb0();
				}
				else {
					tileSpriteIndex = sprite->getTileIndex1();
					tileSpriteRgb = sprite->getTileRgb1();
				}
			}
			else {
				if (verticalFlip == 0) {
					tileSpriteIndex = sprite->getTileIndex1();
					tileSpriteRgb = sprite->getTileRgb1();
				}
				else {
					tileSpriteIndex = sprite->getTileIndex0();
					tileSpriteRgb = sprite->getTileRgb0();
				}
				sprY = sprY & 0x07;
			}

			// Si está activado el flag de invertir horizontalmente
			if (sprite->getHorizontalFlip())
				sprX = 7 - sprX;

			// Si está activado el flag de invertir verticalmente
			if (verticalFlip)
				sprY = 7 - sprY;
		}

		// Si el pixel del sprite no es vacío
		bool transparent = !(tileSpriteIndex[sprX][sprY] & 0x03);
		if (!transparent) {
			// Pinta el pixel si tiene prioridad sobre las nametables, o en caso contrario si el color de fondo es transparente
			if ((pixelBck != 2) && (sprite->getPriority() == 0 || !pixelBck))
				gfx->drawPixel(screenX, screenY, tileSpriteRgb[sprX][sprY]);

			pixelBackground[screenX][screenY] = 2;
		}//if
	}//if
}//drawSpritePixel()


void PPU::getSpritesList() {
	int n = 0;
	int addr = 0x00;

	while (n < 64) {
		spritesList[n]->loadByAddr(spriteMemory, addr);
		addr += 0x04;
		n += 1;
	}
}//getSpritesList()


void PPU::calcSpriteHit() {
	int spritesSizeBit = control1SpritesSizeBit5();
	if (spriteZero->isInScanline(scanlineNumber, spritesSizeBit)) {
		int y = scanlineNumber - 1;
		int offsetX = spriteZero->getOffsetX();
		int sprY = y - spriteZero->getOffsetY();

		int** tileIndex;
		RGB** tileRgb;

		if (spritesSizeBit == 0) {
			tileIndex = spriteZero->getTileIndex0();
			tileRgb = spriteZero->getTileRgb0();
		}
		else {
			if (sprY < 8) {
				tileIndex = spriteZero->getTileIndex0();
				tileRgb = spriteZero->getTileRgb0();
			}
			else {
				tileIndex = spriteZero->getTileIndex1();
				tileRgb = spriteZero->getTileRgb1();
			}
		}

		sprY = sprY & 0x07;

		int screenX = 0;
		// Calcula si hay alguna colisión
		for (int sprX = 0; sprX < 8; sprX++) {
			screenX = offsetX + sprX;
			if (screenX < 255) {
				if ((tileIndex[sprX][sprY] & (0x03 != 0)) && (pixelBackground[screenX][y] == 1))
					setSpriteHit(1);
			}//if
		}//for
	}//if
}//calcSpriteHit()


// Devuelve el color almacenado en la attr table para el tile de una dirección de la name table
int PPU::calcAttrColor(int nametableAddr) {
	// La posición de la name table en la que está el tile
	int pos = nametableAddr & 0x03FF;

	// Posición del grupo de 8x8 tiles que contiene el tile con el que estamos trabajando
	int groupX = (pos >> 2) & 0x07;
	int groupY = (pos >> 7) & 0x07;

	// Posición del byte en la "attr table" correspondiente al grupo 8x8
	int attrPos = (groupY * 8) + groupX;

	// Dirección de memoria de la "attr table"
	int attrAddr = (nametableAddr & 0xFC00)  | (0x03C0 + attrPos);

	// Byte leído de la attr table que contiene la info de color
	int attrData = memory->readData(attrAddr);

	// Calculamos la posición relativa del tile dentro del grupo 8x8
	// La posición x dentro del grupo 4x4 la dan los 2 bits menos significativos de la posición del tile en la name table
	int posX = pos & 0x03;

	// La posición y dentro del grupo 4x4 la dan los bits 5 y 6 de la posición del tile en la name table
	int posY = (pos >> 5) & 0x03;

	// Calcula el area que le corresponde al tile dentro del grupo 4x4
	int attrArea = (posY & 0x02) | (posX >> 1);

	int color = (attrData >> (attrArea << 1)) & 0x03;

	return color;
}//calcAttrColor()


// Devuelve un color de la paleta de colores
RGB PPU::getColor(int index) {
	return PPU::COLOR_PALETTE[index & 0x3F];
}


// Lee el patrón "pattern_index" de la tabla de patrones "pattern_table" con el color "attr_color" y la paleta
// de colores ubicada en la dirección de memoria "palette_addr" y lo coloca en las variables de salida
// "tile_palette_index" y "tile_rgb"
void PPU::fetchPattern(int patternTable, int patternIndex, int attrColor,
	int paletteAddr, int** tileIndex, RGB** tileRgb) {

	int addr;
	if (patternTable == 0)
		addr = 0x0000;
	else
		addr = 0x1000;

	addr = addr + patternIndex * 16;

	int y = 0;
	int byte1, byte2;
	// Lee los bytes del patrón de memoria y lo guarda en una lista
	for (int a = addr; a < addr + 8; a++) {

		byte1 = memory->readData(a);
		byte2 = memory->readData(a + 8);

		int paletteIndex;
		for (int x = 0; x < 8; x++) {
			// Calcula la dirección del color en la paleta de memoria y lo extrae de la tabla de colores
			paletteIndex = (0x00 | ((byte1 & (0x01 << x)) >> x) | (((byte2 & (0x01 << x)) >> x) << 1) | ((attrColor & 0x03) << 2));

			// Asigna el índice de la paleta a la posición correspondiente:
			tileIndex[7 - x][y] = paletteIndex;

			int colorIndex = memory->readData(paletteAddr + paletteIndex) & 0x3F;
			RGB rgb = getColor(colorIndex);
			tileRgb[7 - x][y] = rgb;
		}

		y = (y + 1) & 0x07;
	}
}//fetchPattern()


void PPU::resetTilesCache() {
	// FIXME: probablemente esto no libere bien la memoria. Hay que hacerlo
	// correctamente porque se reserva memoria para cada tile que se carga
	tilesCache.clear();
}


const RGB PPU::COLOR_PALETTE[] = {
										(0x75, 0x75, 0x75),    //0x00
										(0x27, 0x1B, 0x8F),    //0x01
										(0x00, 0x00, 0xAB),    //0x02
										(0x47, 0x00, 0x9F),    //0x03
										(0x8F, 0x00, 0x77),    //0x04
										(0xAB, 0x00, 0x13),    //0x05
										(0xA7, 0x00, 0x00),    //0x06
										(0x7F, 0x0B, 0x00),    //0x07
										(0x43, 0x2F, 0x00),    //0x08
										(0x00, 0x47, 0x00),    //0x09
										(0x00, 0x51, 0x00),    //0x0A
										(0x00, 0x3F, 0x17),    //0x0B
										(0x1B, 0x3F, 0x5F),    //0x0C
										(0x00, 0x00, 0x00),    //0x0D
										(0x00, 0x00, 0x00),    //0xOE
										(0x00, 0x00, 0x00),    //0x0F
										(0xBC, 0xBC, 0xBC),    //0x10
										(0x00, 0x73, 0xEF),    //0x11
										(0x23, 0x3B, 0xEF),    //0x12
										(0x83, 0x00, 0xF3),    //0x13
										(0xBF, 0x00, 0xBF),    //0x14
										(0xE7, 0x00, 0x5B),    //0x15
										(0xDB, 0x2B, 0x00),    //0x16
										(0xCB, 0x4F, 0x0F),    //0x17
										(0x8B, 0x73, 0x00),    //0x18
										(0x00, 0x97, 0x00),    //0x19
										(0x00, 0xAB, 0x00),    //0x1A
										(0x00, 0x93, 0x3B),    //0x1B
										(0x00, 0x83, 0x8B),    //0x1C
										(0x00, 0x00, 0x00),    //0x1D
										(0x00, 0x00, 0x00),    //0x1E
										(0x00, 0x00, 0x00),    //0x1F
										(0xFF, 0xFF, 0xFF),    //0x20
										(0x3F, 0xBF, 0xFF),    //0x21
										(0x5F, 0x97, 0xFF),    //0x22
										(0xA7, 0x8B, 0xFD),    //0x23
										(0xF7, 0x7B, 0xFF),    //0x24
										(0xFF, 0x77, 0xB7),    //0x25
										(0xFF, 0x77, 0x63),    //0x26
										(0xFF, 0x9B, 0x3B),    //0x27
										(0xF3, 0xBF, 0x3F),    //0x28
										(0x83, 0xD3, 0x13),    //0x29
										(0x4F, 0xDF, 0x4B),    //0x2A
										(0x58, 0xF8, 0x98),    //0x2B
										(0x00, 0xEB, 0xDB),    //0x2C
										(0x00, 0x00, 0x00),    //0x2D
										(0x00, 0x00, 0x00),    //0x2E
										(0x00, 0x00, 0x00),    //0x2F
										(0xFF, 0xFF, 0xFF),    //0x30
										(0xAB, 0xE7, 0xFF),    //0x31
										(0xC7, 0xD7, 0xFF),    //0x32
										(0xD7, 0xCB, 0xFF),    //0x33
										(0xFF, 0xC7, 0xFF),    //0x34
										(0xFF, 0xC7, 0xDB),    //0x35
										(0xFF, 0xBF, 0xB3),    //0x36
										(0xFF, 0xDB, 0xAB),    //0x37
										(0xFF, 0xE7, 0xA3),    //0x38
										(0xE3, 0xFF, 0xA3),    //0x39
										(0xAB, 0xF3, 0xBF),    //0x3A
										(0xB3, 0xFF, 0xCF),    //0x3B
										(0x9F, 0xFF, 0xF3),    //0x3C
										(0x00, 0x00, 0x00),    //0x3D
										(0x00, 0x00, 0x00),    //0x3E
										(0x00, 0x00, 0x00)     //0x3F
};
