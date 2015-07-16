/*
 * CPU.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "CPU.hpp"
#include "../nesutils.hpp"
#include "../ppu/PPU.hpp"

/* MÉTODOS PÚBLICOS */

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

	// Pool de instrucciones
	instructionsPool = new InstructionsPool(this);
}//CPU()

CPU::~CPU() {
	delete instructionsPool;
}//~CPU()


// procesa una interrupción IRQ
void CPU::interruptIrq() {
	irq = 0;
	interrupt(CPU::INT_ADDR_IRQ);
}//interruptIrq()


// Procesa una interrupción VBLANK
void CPU::interruptVblank() {
	ppu->setIntVblank(false);
	interrupt(CPU::INT_ADDR_VBLANK);
}//InterruptVblank()


// Indica si hay una IRQ pendiente
bool CPU::getIrq() {
	return irq;
}//getIrq()


// Devuelve una referencia a la memoria
Memory* CPU::getMem() {
	return mem;
}//getMem()


// Activa o desactiva una IRQ
void CPU::setIrq(bool v) {
	irq = v;
}//setIrq()


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


// Incrementa el registro PC en el valor indicado por n
void CPU::incrPc(int n) {
	regPc = (regPc + 1) & 0xFFFF;
}//incrPc()


// Devuelve la siguiente instrucción a ejecutar
Instruction* CPU::fetchInst() {
	int opcode = mem->readData(regPc);

	// Los comentarios de a continuación son por rendimiento
	Instruction* inst = instructionsPool->pool[opcode];

	int bytes = inst->getBytes();
	int operand;

	if (bytes == 2)                  // Instrucciones con operando de 1 byte
	{
		operand = mem->readData(regPc + 1);
		inst->setOperand(operand);
	}
	else if (bytes == 3) {                // Instrucciones con operando de 2 bytes
		operand = mem->readData(regPc + 1);
		operand = operand | (mem->readData(regPc + 2) << 8);
		inst->setOperand(operand);
	}

	return inst;
}


// Métodos que devuelven el valor de los bits de los registros de estado
int CPU::getReg_p_c_bit() {
	return getBit(regP, CPU::REG_P_BIT_C);
}


int CPU::getReg_p_z_bit() {
	return getBit(regP, CPU::REG_P_BIT_Z);
}


int CPU::getReg_p_i_bit() {
	return getBit(regP, CPU::REG_P_BIT_I);
}


int CPU::getReg_p_d_bit() {
	return getBit(regP, CPU::REG_P_BIT_D);
}


int CPU::getReg_p_b_bit() {
	return getBit(regP, CPU::REG_P_BIT_B);
}


int CPU::getReg_p_v_bit() {
	return getBit(regP, CPU::REG_P_BIT_V);
}


int CPU::getReg_p_s_bit() {
	return getBit(regP, CPU::REG_P_BIT_S);
}


// Métodos que establecen el valor de los bits delregistro de estado
void CPU::setReg_p_c_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_C, v);
}


void CPU::setReg_p_z_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_Z, v);
}


void CPU::setReg_p_i_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_I, v);
}


void CPU::setReg_p_d_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_D, v);
}


void CPU::setReg_p_b_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_B, v);
}


void CPU::setReg_p_v_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_V, v);
}


void CPU::setReg_p_s_bit(int v) {
	setBit(regP, CPU::REG_P_BIT_S, v);
}


// Calcula y establece el valor del bit de carry en función del resultado de una instrucción
int CPU::setCarryBit(int instResult) {
	if (0 <= instResult < 0x100) {
		setReg_p_c_bit(0);
		return 0;
	}
	else {
		setReg_p_c_bit(1);
		return 1;
	}
}


int CPU::setZeroBit(int instResult) {
	int rst = instResult & 0xFF;
	if (rst == 0x00) {
		setReg_p_z_bit(1);
		return 1;
	}
	else {
		setReg_p_z_bit(0);
		return 0;
	}
}


int CPU::setSignBit(int instResult) {
	int rst = instResult & 0xFF;
	if (rst & 0x80) {
		setReg_p_s_bit(1);
		return 1;
	}
	else {
		setReg_p_s_bit(0);
		return 0;
	}
}


void CPU::pushStack(int byte) {
    int spAddr = 0x0100 | getRegSp();
    getMem()->writeData(byte, spAddr);
    setRegSp(getRegSp() - 1);
}


int CPU::pullStack() {
    int spAddr = 0x0100 | (getRegSp() + 1);
    int byte = getMem()->readData(spAddr);
    setRegSp(getRegSp() + 1);
    return byte;
}


/* MÉTODOS PRIVADOS */


// Procesa una interrupción
void CPU::interrupt(int vectorAddr) {
	pushStack((regPc >> 8) & 0xFF);
	pushStack(regPc & 0xFF);
	// En las interrupciones el bit 4 se pone a 0 y el 5 a 1
	pushStack((regP & 0xEF) | 0x20);
	setReg_p_i_bit(1);
	int addr = mem->readData(vectorAddr) & 0xFF;
	addr = addr | (mem->readData(vectorAddr + 1) << 8);
	regPc = addr;
}
