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

class Memory;
class Sprite;


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


// Estructura para almacenar la información de un tile en sus dos formatos
struct TilePair {
	int** tileIndex;
	RGB** tileRgb;
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

	PPU(Mapper *mapper);
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
	PPUMemory* memory;
	SpriteMemory* spriteMemory;
	Mapper* mapper;
	GFX* gfx;

	int cyclesFrame;		// Ciclos restantes hasta el siguiente frame

	bool endFrame;			// Indica si se ha terminado el frame
	bool endScanline;		// Indica si se ha terminado el scanline

	bool newPattern;		// Indica si tenemos que leer un nuevo tile de memoria o usamos el cacheado

	int scanlineNumber;		// Número del scanline actual
	int scanlinesPending;	// Número de scanlines pendientes de procesar

	Sprite* spritesList[64];	// Lista de sprites en el frame actual
	Sprite* spriteZero;		// Referencia al sprite zero

	int tileSpriteZeroIndex0[8][8];
	RGB tileSpriteZeroRgb0[8][8];

	int tileSpriteZeroIndex1[8][8];
	RGB tileSpriteZeroRgb1[8][8];

	bool spriteHit;			// Indica si ha habido colisión de sprite en el frame actual

	int pixelBackground[256][240];	// Indica si el pixel es de background transparente(0), de background sólido (1) o de sprite (2)

	bool intVblank;		// Flag de VBlank (se activa cuando se entra en vblank)

	// Buffer de lectura de la VRAM (la lectura del registro $2007 se entrega retrasada)
	int vramBuffer;

	// Variables que almacenan el tile del fondo que se está procesando
	int tileBgIndex[8][8];
	RGB tileBgRgb[8][8];

	// Variables que almacenan el tile del sprite que se está procesando
    int tileSpriteIndex[8][8];
    RGB tileSpriteRgb[8][8];

    // Cache de tiles para mejorar rendimiento
    map<TileCacheKey, TilePair> tilesCache;

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

    // Devuelve un color de la paleta de colores
    RGB getColor(int index);

    // Borra y libera la memoria de la cache de tiles
    void resetTilesCache();

	// Lee el patrón "patternIndex" de la tabla de patrones "patternTable" con el color "attrColor" y la paleta
    // de colores ubicada en la dirección de memoria "paletteAddr" y lo coloca en las variables de salida
    // "tileIndex" y "tileRgb"
    void fetchPattern(int patternTable, int patternIndex, int attrColor, int paletteAddr, int** tileIndex, RGB** tileRGB);

    // Paleta de colores:
    static const RGB COLOR_PALETTE[] = {
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

};

#endif /* PPU_PPU_HPP_ */
