/*
 * Instruction.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "Instruction.hpp"
#include "CPU.hpp"

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


/******************************************************************************
 * ADC: Add memory to accumulator with carry
 *****************************************************************************/

ADC::ADC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int ADC::execute(int op) {
	int ac = cpu->getRegA();
	int carry = cpu->getReg_p_c_bit();

	int rst = ac + op + carry;

	// Establece el bit CARRY del registro P
	cpu->setCarryBit(rst);

	// Establece el bit ZERO del registro P
	cpu->setZeroBit(rst);

	// Establece el bit OVERFLOW del registro P
	if ((!((ac ^ op) & 0x80)) && ((ac ^ rst) & 0x80))
		cpu->setReg_p_v_bit(1);
	else
		cpu->setReg_p_v_bit(0);

	// Establece el bit SIGN del registro P
	cpu->setSignBit(rst);

	cpu->setRegA(rst);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ADC_inmediate
ADC_inmediate::ADC_inmediate(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_inmediate::execute() {
	return ADC::execute(operand);
}


// ADC_zero
ADC_zero::ADC_zero(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_zerox
ADC_zerox::ADC_zerox(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_abs
ADC_abs::ADC_abs(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_absx
ADC_absx::ADC_absx(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_absy
ADC_absy::ADC_absy(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_preindexi
ADC_preindexi::ADC_preindexi(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_postindexi
ADC_postindexi::ADC_postindexi(int operand, CPU* cpu) : ADC(operand, cpu) {}

int ADC_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


/******************************************************************************
 * AND: And memory with accumulator
 *****************************************************************************/

AND::AND(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int AND::execute(int op) {
	int ac = cpu->getRegA();
	int rst = ac & op;

	// Establece el bit SIGN del registro P
	cpu->setSignBit(rst);
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(rst);

	cpu->setRegA(rst);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// AND_inmediate
AND_inmediate::AND_inmediate(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_inmediate::execute() {
	return AND::execute(operand);
}


// AND_zero
AND_zero::AND_zero(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_zerox
AND_zerox::AND_zerox(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_abs
AND_abs::AND_abs(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_absx
AND_absx::AND_absx(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_absy
AND_absy::AND_absy(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_preindexi
AND_preindexi::AND_preindexi(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_postindexi
AND_postindexi::AND_postindexi(int operand, CPU* cpu) : AND(operand, cpu) {}

int AND_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


/******************************************************************************
 * ASL: Shift Left One Bit (Memory or Accumulator)
 *****************************************************************************/

ASL::ASL(int operando, CPU* cpu) : Instruction(operando, cpu) {}

int ASL::execute(int op) {
	int result = op << 1;

	// Establece el bit SIGN del registro P
	cpu->setCarryBit(result);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(result);
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(result);

	return result & 0xFF;
}


// ASL_accumulator
ASL_accumulator::ASL_accumulator(int operando, CPU* cpu) : ASL(operando, cpu) {}

int ASL_accumulator::execute() {
	int result = ASL::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ASL_zero
ASL_zero::ASL_zero(int operando, CPU* cpu) : ASL(operando, cpu) {}

int ASL_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ASL_zerox
ASL_zerox::ASL_zerox(int operando, CPU* cpu) : ASL(operando, cpu) {}

int ASL_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ASL_abs
ASL_abs::ASL_abs(int operando, CPU* cpu) : ASL(operando, cpu) {}

int ASL_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ASL_absx
ASL_absx::ASL_absx(int operando, CPU* cpu) : ASL(operando, cpu) {}

int ASL_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}
