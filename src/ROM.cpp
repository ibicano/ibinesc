/*
 * ROM.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "ROM.hpp"

#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>

using namespace std;


ROM::ROM(string fileName) {
	prgCount = 0;
	chrCount = 0;
	control1 = 0x00;
	control2 = 0x00;
	ramCount = 0;

	loadOk = false;

	loadFile(fileName);
}

ROM::~ROM() {
	for (unsigned int i = 0; i < prgCount; i++) {
		if (prgBanks[i] != NULL) delete []prgBanks[i];
	}
	delete []prgBanks;

	for (unsigned int i = 0; i < chrCount; i++) {
		if (chrBanks[i] != NULL) delete []chrBanks[i];
	}
	delete []chrBanks;
}

void ROM::loadFile(string fileName) {

	// Lee el fichero de la ROM y lo almacena en un vector en memoria para poder parsearlo
	// FIXME: NO CARGA BIEN LA ROM. ARREGLARLO

	ifstream f(fileName.c_str());

	char byte;
	while (f.get(byte)) {
		rom.push_back((unsigned char)byte);
	}
	f.close();

	string s = "";
	for (int i= 0; i < 3; i++) {
		s = s + (char)rom[i];
	}

	if (s == "NES" && (unsigned int)rom[3] == 0x1A) {
		// Carga la cabecera
		prgCount = rom[4];
		chrCount = rom[5];
		control1 = rom[6];
		control2 = rom[7];
		ramCount = rom[8];
		//reserved = rom[9:16]


		// Carga el modo de mirroring
		if (getControl1MirroringBit3() == 0)
			mirroring = getControl1MirroringBit0();
		else
			mirroring = 0x03;

		// Cargamos los datos de la ROM. La variable "i" es el índice de lectura
		int i = 16;		// Empezamos después de las cabeceras
		int end = i + 512;

		// Si hay un trainer se carga primero
		if (getControl1TrainerBit2()) {
			while (i < end) {
				trainer[i] = rom[i];
				i++;
			}//while
		}//if

		prgBanks = new unsigned int*[prgCount];
		// Carga los bancos PRG
		for (unsigned int n = 0; n < prgCount; n++) {
			prgBanks[n] = new unsigned int[ROM::PRG_SIZE];

			int j = 0;
			while (j < 16384) {
				prgBanks[n][j] = rom[i];
				i++;
				j++;
			}//while
		}//for

		chrBanks = new unsigned int*[chrCount];
		// Carga los bancos CHR
		for (unsigned int n = 0; n < chrCount; n++) {
			chrBanks[n] = new unsigned int[ROM::CHR_SIZE];

			int j = 0;
			while (j < 8192) {
				chrBanks[n][j] = rom[i];
				i++;
				j++;
			}//while
		}//for

		loadOk = true;
	}//if
	else {
		loadOk = false;
		cout << "Formato de fichero incorrecto";
	}//else
}//loadFile()


/* Devuelve el modo mirroring especificado en la ROM:
   0x00: horizontal
   0x01: vertical
   0x02: single
   0x03: 4-screen
*/
int ROM::getMirroring() {
	return mirroring;
}


int ROM::getMapperCode() {
	int mapperCode = control1 >> 4;
	mapperCode = mapperCode | (control2 & 0xF0);

	return mapperCode;
}


int ROM::getPrgCount() {
	return prgCount;
}


int ROM::getChrCount() {
	return chrCount;
}


unsigned int* ROM::getPrg(int n) {
	return prgBanks[n];
}


unsigned int* ROM::getPrg8k(int n) {
	int number = n >> 1;
	int part = n & 0x01;

	unsigned int* bank;

	if (part == 0)
		bank = &prgBanks[number][0];
	else
		bank = &prgBanks[number][8192];

	return bank;
}


unsigned int* ROM::getChr(int n) {
	return chrBanks[n];
}


unsigned int* ROM::getChr4k(int n) {
	int number = n / 2;
	unsigned int* bank;

	// Si es par es la primera mitad del banco de 8k
	if ((n & 0x01) == 0)
		bank = &chrBanks[number][0x0000];
	else
		bank = &chrBanks[number][0x1000];

	return bank;
}


unsigned int* ROM::getChr1k(int n) {
	int number = n >> 3;
	int part = n & 0x07;
	int addr = part * 1024;

	unsigned int* bank = &chrBanks[number][addr];

	return bank;
}


int ROM::getControl1MirroringBit0() {
	return control1 & 0x01;
}


int ROM::getControl1MirroringBit3() {
	return (control1 & 0x08) >> 3;
}


int ROM::getControl1TrainerBit2() {
	return (control1 & 0x04) >> 2;
}
