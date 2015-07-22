/*
 * MMC3.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_MMC3_HPP_
#define MAPPERS_MMC3_HPP_

#include "Mapper.hpp"

class ROM;
class CPU;

class MMC3 : public Mapper {
public:
	static const int MAPPER_CODE = 4;

	MMC3(ROM* rom);
	virtual ~MMC3();

	int getMirrorMode();

	int readPrg(int addr);
	int readChr(int addr);

	void writePrg(int data, int addr);
	void writeChr(int data, int addr);

	void setCpu(CPU* cpu);

	void scanlineTick();

private:
	// Referencia a la cpu
	CPU* cpu;

	// Número de bancos prg de 8k
	int prgCount8k;

	// Bancos prg de 8k
	unsigned int* prgRom0;
	unsigned int* prgRom1;
	unsigned int* prgRom2;
	unsigned int* prgRom3;

	// Bancos chr de 1k
	unsigned int* chrRom0;
	unsigned int* chrRom1;
	unsigned int* chrRom2;
	unsigned int* chrRom3;
	unsigned int* chrRom4;
	unsigned int* chrRom5;
	unsigned int* chrRom6;
	unsigned int* chrRom7;

	// Mirror mode
	int mirrorMode;

	// Estado de save RAM
	int saveRam;

	// Banco seleccionado
	int bankSelect;

	// Modo swap
	int bankMode;

	// Indica si se debe hacer XOR 0x1000 cuando bank_select es 0-5
	bool bankInversion;

	// Registros de datos
	int r0;
	int r1;
	int r2;
	int r3;
	int r4;
	int r5;
	int r6;
	int r7;

	// Registros relacionados con las IRQ
    bool irqEnable;
    bool irqReloadFlag;			// Indica si se recarga el irq_counter con irq_latch
    int irqLatch;        		// Valor con el que recargar el irq_counter
    int irqCounter;          	// Contador de scanlines que va decrementándose y cuando llega a 0 lanza una IRQ


    // Intercambia los bancos
    void swapBanks();
};

#endif /* MAPPERS_MMC3_HPP_ */
