/*
 * PPU.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_PPU_HPP_
#define PPU_PPU_HPP_

class Memory;
class Sprite;

struct RGB {
	int r;
	int g;
	int b;
};

class PPU {
public:
	static const int FRAME_CYCLES = 33072;
	static const int SCANLINE_CYCLES = 106;
	static const int VBLANK_CYCLES = 7420;
	static const int FRAME_SCANLINES = 312;
	static const int VBLANK_SCANLINES = 70;
	static const int FRAME_PERIOD = 0.020;        // Periodo de frame en segundos

	static const int FRAME_WIDTH = 256;
	static const int FRAME_HEIGHT = 240;

	PPU();
	virtual ~PPU();

	// Ejecuta los ciclos indicados en la PPU
	void execCycles(int cycles);

	// Devuelve el valor del registro indicado por su dirección mapeada en memoria
	int readReg(int addr);

	// Escribe el registro indicado por su dirección mapeada en memoria
	void writeReg(int data, int addr);

	// Lee y establece el valor del indicador de vblank
	bool getIntVblank();
	void setIntVblank(bool v);

private:
	int readReg2002();		// Devuelve el contenido del registro $2002
	int readReg2004();		// Devuelve el contenido del registro $2004
	int readReg2007();		// Devuelve el contenido del registro $2007

	void writeReg2000();	// Escribe el registro $2000
	void writeReg2001();	// Escribe el registro $2001
	void writeReg2003();	// Escribe el registro $2003
	void writeReg2004();	// Escribe el registro $2004
	void writeReg2005();	// Escribe el registro $2005
	void writeReg2006();	// Escribe el registro $2006
	void writeReg2007();	// Escribe el registro $2007

	// Realiza una transferencia DMA que llena la memoria de sprites con las los
	// datos contenidos en las 256 posiciones de memoria consecutivas de la memoria
	// principal pasada como "cpuMem" empezando en la dirección "srcAddr"
	void writeSpriteDma(Memory* cpuMem, int srcAddr);

	// Se usa para actualiza los registros según se dibuja
	void incrXScroll();
	void incrYScroll();

	// Devuelve el valor de 8 bits del registro de desplazamiento x del scroll de un tile
    int getXOffset();

	// Devuelve el valor de 8 bits del registro de desplazamiento y del scroll de un tile
    int getYOffset();

	// Métodos para obtener información de los registros de control
    int control1NametableBits0_1();

	// Devuelve el valor del bit que indica el incrmento de dirección
    // 1, si es 0 o 32 si es 1
    int control1IncrementBit2();

    int control1SpritesPatternBit3();

    int control1BackgroundPatternBit4();

    int control1SpritesSizeBit5();

    int control1MasterModeBit6();

    int control1NmiBit7();

    int control2MonochromeBit0();

    int control2ClipbackgroundBit1();

    int control2ClipspritesBit2();

    int control2BackgroundBit3();

    int control2SpritesBit4();

    int control2ColourconfigBits5_7();

	// Devuelve la dirección de la Name Table activa en los bits 0-1 del registro de control 1
    int activeNametableAddr();

	// Inicia un período VBLANK
    int startVblank();

	// Finaliza un período VBLANK
    int endVblank();

    // Establece el flag sprite hit del registro de status
    int setSpriteHit(bool v);

	// Indica si nos encontramos en un periodo VBLANK
    bool isVblank();

    void drawScanline();

    // Dibuja un pixel en la pantalla
    void drawPixel(int x, int y);

    // Dibuja todos los sprites
    void drawSprites();

	// Dibuja el pixel (spr_x, spr_y) del sprite en pantalla en su posición correspondiente
    void drawSpritePixel(Sprite* sprite, int sprX, int sprY);

	// Devuelve una lista de objetos de clase Sprite con los sprites de la memoria de sprites
    void getSpritesList();

    // Calcula si hay colisión del sprite Zero en el scanline procesado actualmente
    void calcSpriteHit();

    // Devuelve el color almacenado en la attr table para el tile de una dirección de la name table
    int calcAttrColor(int nametableAddr);


	// Lee el patrón "patternIndex" de la tabla de patrones "patternTable" con el color "attrColor" y la paleta
    // de colores ubicada en la dirección de memoria "paletteAddr" y lo coloca en las variables de salida
    // "tileIndex" y "tileRgb"
    void fetchPattern(int patternTable, int patternIndex, int attrColor, int paletteAddr, int** tileIndex, RGB** tileRGB);

};

#endif /* PPU_PPU_HPP_ */
