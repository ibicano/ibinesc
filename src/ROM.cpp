/*
 * ROM.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "ROM.hpp"

#include <fstream>
#include <iostream>


ROM::ROM(string fileName) {
	prgCount = 0;
	chrCount = 0;
	control1 = 0x00;
	control2 = 0x00;
	ramCount = 0;

	prgBanks = new int*[prgCount];
	chrBanks = new int*[chrCount];
	chrBanks = new int*[ramCount];

	loadOk = false;

	loadFile(fileName);
}

ROM::~ROM() {
	for (int i = 0; i < prgCount; i++) {
		if (prgBanks[i] != NULL) delete []prgBanks[i];
	}

	for (int i = 0; i < chrCount; i++) {
		if (chrBanks[i] != NULL) delete []chrBanks[i];
	}
}

int ROM::loadFile(string fileName) {

	// Lee el fichero de la ROM y lo almacena en un vector en memoria para poder parsearlo
	ifstream f(fileName);
	char byte;
	while (f.get(byte)) {
		rom.push_back(byte);
	}
	f.close();

	string s = "";
	for (int i= 0; i < 3; i++) {
		s = s + rom[i];
	}

	if (s == "NES" && (unsigned int)rom[3] == 0x1A) {
		// Carga la cabecera
		prgCount = rom[4];
		chrCount = rom[5];
		control1 = rom[6];
		control2 = rom[7];
		ramCount = rom[8];
		//reserved = rom[9:16]

		// Reserva la memoria para almacenar los bancos en función del número de estos
		for (int i = 0; i < prgCount; i++) {
			if (prgBanks[i] != NULL) delete []prgBanks[i];
			prgBanks[i] = new int[ROM::PRG_SIZE];
		}

		for (int i = 0; i < chrCount; i++) {
			if (chrBanks[i] != NULL) delete []chrBanks[i];
			chrBanks[i] = new int[ROM::CHR_SIZE];
		}

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

		// Carga los bancos PRG
		for (int n = 0; n < prgCount; n++) {
			prgBanks[n] = new int[16384];

			int j = 0;
			while (j < 16384) {
				prgBanks[n][j] = rom[i];
				i++;
				j++;
			}//while
		}//for

		// Carga los bancos CHR
		for (int n = 0; n < chrCount; n++) {
			chrBanks[n] = new int[8192];

			int j = 0;
			while (j < 8192) {
				prgBanks[n][j] = rom[i];
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


int* ROM::getPrg(int n) {
	return prgBanks[n];
}


int* ROM::getPrg8k(int n) {
	int number = n >> 1;
	int part = n & 0x01;

	int* bank;

	if (part == 0)
		bank = &prgBanks[number][0];
	else
		bank = &prgBanks[number][8192];

	return bank;
}


int* ROM::getChr(int n) {
	return chrBanks[n];
}


int* ROM::getChr4k(int n) {
	int number = n / 2;
	int* bank;

	// Si es par es la primera mitad del banco de 8k
	if (n & 0x01 == 0)
		bank = &chrBanks[number][0x0000];
	else
		bank = &chrBanks[number][0x1000];

	return bank;
}


int* ROM::getChr1k(int n) {
	int number = n >> 3;
	int part = n & 0x07;
	int addr = part * 1024;

	int* bank = &chrBanks[number][addr];

	return bank;
}


int ROM::getControl1MirroringBit0() {
	return control1 & 0x01;
}


int ROM::getControl1MirroringBit3() {
	return control1 & 0x08 >> 3;
}


int ROM::getControl1TrainerBit2() {
	return control1 & 0x04 >> 2;
}
