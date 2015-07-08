/*
 * CPU.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "CPU.hpp"

// Constructor
CPU::CPU(Memory* mem, PPU* ppu) {
	this->mem = mem;
	this->ppu = ppu;

	// Inicializa los registros
	regPc = mem->readData(CPU::INT_ADDR_RESET);
	regPc = regPc | (mem->readData(CPU::INT_ADDR_RESET + 1) << 8);

	regSp = 0xFF;
	regA = 0x00;
	regX = 0x00;
	regY = 0x00;
	regP = 0x34;

	// Inicializa el flag de IRQ
	irq = false;
}//CPU()

CPU::~CPU() {
}//~CPU()


// TODO: implementar esta función
Instruction* CPU::fetchInst() {
	return NULL;
}//fetchInst()


int CPU::getRegPc() {
	return regPc;
}//getRegPc()


int CPU::getRegSp() {
	return regSp;
}//getRegSp()


int CPU::getRegA() {
	return regA;
}//getRegA()


int CPU::getRegX() {
	return regX;
}//getRegX()


int CPU::getRegY() {
	return regY;
}//getRegY()


int CPU::getRegP() {
	return regP;
}//getRegP()


void CPU::setRegPc(int v) {
	regPc = v;
}


void CPU::setRegSp(int v) {
	regSp = v;
}


void CPU::setRegA(int v) {
	regA = v;
}


void CPU::setRegX(int v) {
	regX = v;
}


void CPU::setRegY(int v) {
	regY = v;
}


void CPU::setRegP(int v) {
	regP = v;
}
