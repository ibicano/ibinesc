/*
 * ROM.h
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef ROM_HPP_
#define ROM_HPP_

#include <string>
#include <vector>

using namespace std;

class ROM {
public:
	static const int PRG_SIZE = 16384;
	static const int CHR_SIZE = 8192;

	ROM(string fileName);
	virtual ~ROM();

	// Carga la ROM de un fichero
	int loadFile(string fileName);

	/*
	 * Devuelve el modo de mirroring indicado en la ROM:
	 * 0: horizontal
	 * 1: vertical
	 * 2: single
	 * 3: 4-screen
	 */
	int getMirroring();

	// Devuelve el código del Mapper de la ROM
	int getMapperCode();

	// Devuelve el número de bancos de programa de 16KB presentes en la ROM
	int getPrgCount();

	// Devuelve el número de bancos de patrones de 8KB presentes en la ROM
	int getChrCount();

	// Devuelve el banco PRG número n
	int* getPrg(int n);

	// Devuelve un banco PRG con direccionamiento como si fuesen de 8k en vez de 16k
    int* getPrg8k(int n);

	// Devuelve el banco CHR número n
	int* getChr(int n);

	// Devuelve un banco CHR con direccionamiento como si fuesen de 4k en vez de 8k
    int* getChr4k(int n);

    // Devuelve un banco CHR con direccionamiento como si fuesen de 1k en vez de 8k
    int* getChr1k(int n);

	//TODO: Acabar la especificación de las funciones

private:
	// Contenido de la ROM
	vector<char> rom;

	int prgCount;		// Número de bancos de 16KB de ROM de programa
	int chrCount;		// Número de bancos de 8KB de ROM de patrones
	int control1;		// Byte de control 1
	int control2;		// Byte de control 2
	int ramCount;		// Bytes de memoria RAM

	// Array que almacena el trainer, en caso de que lo haya
	int trainer[512];

	// Arrays que almacenan los bancos de rom
	int** prgBanks;
	int** chrBanks;
	int** ramBanks;

	//Indica si la ROM se ha cargado correctamente
	bool loadOk;

	// Almacena el modo de mirroring indicado en la ROM
	int mirroring;

	// Funciones que devuelven los bits de mirroring del registro de control 1
	int getControl1MirroringBit0();
	int getControl1MirroringBit3();

	// Devuelve el bit que indica si hay un trainer en el registro de control 1
	int getControl1TrainerBit2();

};

#endif /* ROM_HPP_ */
