/*
 * Instruction.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "Instruction.hpp"

Instruction::Instruction(int operand, CPU* cpu) {
	this->operand = operand;
	this->cpu = cpu;
}


int Instruction::fetchAbsoluteAddrmode() {
	return operand;
}


int Instruction::fetchIndexedAbsXAddrmode() {
	return (operand + cpu->getRegX()) & 0xFFFF;
}


int Instruction::fetchIndexedAbsYAddrmode() {
	return (operand + cpu->getRegY()) & 0xFFFF;
}


int Instruction::fetchIndexedZeroXAddrmode() {
	return (operand + cpu->getRegX()) & 0xFF;
}


int Instruction::fetchIndexedZeroYAddrmode() {
	return (operand + cpu->getRegY()) & 0xFF;
}


int Instruction::fetchPreindexedAddrmode() {
	/* Calcula el índice de la dirección donde se almacena la dirección
	final del operando */
	int index = (operand + cpu->getRegX()) & 0xFF;

	// Calcula la dirección final del operando
	int addr = cpu->getMem()->readData(index);
	addr = addr | (cpu->getMem()->readData(((index + 1) & 0xFF)) << 8);

	return addr;
}


int Instruction::fetchPostindexedAddrmode() {
	/* Calcula el índice de la dirección donde se almacena la direccións
	base del operando a la que se sumará el desplazamiento Y */
	int baseAddr = cpu->getMem()->readData(operand);
	baseAddr = baseAddr | (cpu->getMem()->readData((operand + 1) & 0xFF) << 8);

	return (baseAddr + cpu->getRegY()) & 0xFFFF;
}
