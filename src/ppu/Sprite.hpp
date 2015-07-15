/*
 * Sprite.hpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_SPRITE_HPP_
#define PPU_SPRITE_HPP_

class SpriteMemory;
class PPU;

struct RGB;

class Sprite {
public:
	Sprite(PPU* ppu);
	virtual ~Sprite();

	// Carga la información del Sprite por su dirección en la memoria de sprites
	void loadByAddr(SpriteMemory* spriteMem, int addr);

	// Carga la información del Sprite por su posición numérica en la memoria de sprites
	void loadByNumber(SpriteMemory* spriteMem, int number);

	// Devuelve una referencia a la información de los tiles
	int** getTileIndex0();
	RGB** getTileRgb0();

	int** getTileIndex1();
	RGB** getTileRgb1();

	// Devuelve la información del sprite
	int getOffsetY();
	int getIndex();
	int getAttributes();
	int getOffsetX();
	bool getSpriteZero();

	// Devuelve información contenida en el byte de atributos
	int getAttrColor();
	int getPriority();
	int getHorizontalFlip();
	int getVerticalFlip();

	// Indica si el Sprite ocupa la posición del pixel de pantalla indicado
	bool isIn(int x, int y, int sizeBit);

	// Indica si el Sprite está en la línea vertical indicada de la pantalla
	bool isInScanline(int scanline, int sizeBit);

private:
	int offsetY;		// Posición Y en pantalla
	int index;			// Índice de tile en la PatternTable
	int attributes;		// Byte de atributos
	int offsetX;		// Posición X en pantalla
	bool spriteZero;	// Indica si es el Sprite Zero

	PPU* ppu;			// Referencia a la PPU

	// Información de los tiles
	int** tileIndex0;
	RGB** tileRgb0;

	int** tileIndex1;
	RGB** tileRgb1;
};

#endif /* PPU_SPRITE_HPP_ */
