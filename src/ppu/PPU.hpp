/*
 * PPU.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_PPU_HPP_
#define PPU_PPU_HPP_

#include <map>

using namespace std;

class Sprite;
class Mapper;
class PPUMemory;
class Memory;
class SpriteMemory;
class Mapper;
class GFX;
class Tile;


// Estructura que almacena la información de color
struct RGB {
	int r;
	int g;
	int b;
};


// Estructura que se usa como clave para almacenar los tiles
struct TileCacheKey {
	int tableNumber;
	int index;
	int color;
};


class PPU {
public:
	static const int FRAME_CYCLES = 33072;
	static const int SCANLINE_CYCLES = 106;
	static const int VBLANK_CYCLES = 7420;
	static const int FRAME_SCANLINES = 312;
	static const int VBLANK_SCANLINES = 70;
	static const int FRAME_PERIOD = 20;        // Periodo de frame en milisegundos

	static const int FRAME_WIDTH = 256;
	static const int FRAME_HEIGHT = 240;

	PPU(Mapper* mapper);
	virtual ~PPU();

	// Ejecuta los ciclos indicados en la PPU
	bool execCycles(int cycles);

	// Devuelve el valor del registro indicado por su dirección mapeada en memoria
	int readReg(int addr);

	// Escribe el registro indicado por su dirección mapeada en memoria
	void writeReg(int data, int addr);

	// Realiza una transferencia DMA que llena la memoria de sprites con las los
	// datos contenidos en las 256 posiciones de memoria consecutivas de la memoria
	// principal pasada como "cpuMem" empezando en la dirección "srcAddr"
	void writeSpriteDma(Memory* cpuMem, int srcAddr);


	// Lee y establece el valor del indicador de vblank
	bool getIntVblank();
	void setIntVblank(bool v);


	// Lee el patrón "patternIndex" de la tabla de patrones "patternTable" con el color "attrColor" y la paleta
    // de colores ubicada en la dirección de memoria "paletteAddr" y lo coloca en las variables de salida
    // "tileIndex" y "tileRgb"
    Tile* fetchPattern(int patternTable, int patternIndex, int attrColor, int paletteAddr);

	// Borra y libera la memoria de la cache de tiles
	void resetTilesCache();

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

	// Devuelve un puntero al motor gráfico
	GFX* getGfx();

private:
	PPUMemory* memory;
	SpriteMemory* spriteMemory;
	Mapper* mapper;
	GFX* gfx;

	int cyclesFrame;		// Ciclos restantes hasta el siguiente frame

	bool endScanline;		// Indica si se ha terminado el scanline

	bool newPattern;		// Indica si tenemos que leer un nuevo tile de memoria o usamos el cacheado

	int scanlineNumber;		// Número del scanline actual
	int scanlinesPending;	// Número de scanlines pendientes de procesar

	Sprite* spritesList[64];	// Lista de sprites en el frame actual
	Sprite* spriteZero;		// Referencia al sprite zero

	Tile* tileSpriteZero0;

	Tile* tileSpriteZero1;

	bool spriteHit;			// Indica si ha habido colisión de sprite en el frame actual

	int pixelBackground[256][240];	// Indica si el pixel es de background transparente(0), de background sólido (1) o de sprite (2)

	bool intVblank;		// Flag de VBlank (se activa cuando se entra en vblank)

	// Buffer de lectura de la VRAM (la lectura del registro $2007 se entrega retrasada)
	int vramBuffer;

	// Variables que almacenan el tile del fondo que se está procesando
	Tile* tileBg;

	// Variables que almacenan el tile del sprite que se está procesando
    Tile* tileSprite;

    // Cache de tiles para mejorar rendimiento
    map<TileCacheKey, Tile*> tilesCache;

	// Registros I/O
	int regControl1;            // Dirección 0x2000 - write
	int regControl2;            // Dirección 0x2001 - write
	int regStatus;               // Dirección 0x2002 - read
	int regSprAddr;             // Dirección 0x2003 - write
	int regSprIo;               // Dirección 0x2004 - write
	int regVramTmp;             // Dirección 0x2005 y 0x2006 - write (16-bit)
	int regVramAddr;            // Dirección 0x2006 - write (16-bit)
	int regVramIo;              // Dirección 0x2007 - read/write
	int regSpriteDma;           // Dirección 0x4014 - write

	// Registros estado
	int regXOffset;             // Scroll patrón (3-bit)
	int tmpYOffset;             // Alamcena el offset y de forma temporal para leerlo más rápido
	int regVramSwitch;            // Indica si estamos en la 1ª(0) o 2ª(1) escritura de los registros vram


	// Indica si ya se ha inicializado la vblank en este frame. Se resetea al finalizar el frame.
	// Sirve para controlar si ya se ha procesado o no el período VBLANK cuando estamos dentro de
	// él, ya que sólo debe procesarse una vez.
	bool startedVblank;

	int readReg2002();		// Devuelve el contenido del registro $2002
	int readReg2004();		// Devuelve el contenido del registro $2004
	int readReg2007();		// Devuelve el contenido del registro $2007

	void writeReg2000(int data);	// Escribe el registro $2000
	void writeReg2001(int data);	// Escribe el registro $2001
	void writeReg2003(int data);	// Escribe el registro $2003
	void writeReg2004(int data);	// Escribe el registro $2004
	void writeReg2005(int data);	// Escribe el registro $2005
	void writeReg2006(int data);	// Escribe el registro $2006
	void writeReg2007(int data);	// Escribe el registro $2007

	// Se usa para actualiza los registros según se dibuja
	void incrXScroll();
	void incrYScroll();

	// Devuelve el valor de 8 bits del registro de desplazamiento x del scroll de un tile
    int getXOffset();

	// Devuelve el valor de 8 bits del registro de desplazamiento y del scroll de un tile
    int getYOffset();

	// Devuelve la dirección de la Name Table activa en los bits 0-1 del registro de control 1
    int activeNametableAddr();

	// Inicia un período VBLANK
    void startVblank();

	// Finaliza un período VBLANK
    void endVblank();

    // Establece el flag sprite hit del registro de status
    void setSpriteHit(bool v);

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

    // Devuelve un color de la paleta de colores
    RGB getColor(int index);

    // Paleta de colores:
    static const RGB COLOR_PALETTE[];
};

#endif /* PPU_PPU_HPP_ */
