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
	final del operand */
	int index = (operand + cpu->getRegX()) & 0xFF;

	// Calcula la dirección final del operand
	int addr = cpu->getMem()->readData(index);
	addr = addr | (cpu->getMem()->readData(((index + 1) & 0xFF)) << 8);

	return addr;
}


int Instruction::fetchPostindexedAddrmode() {
	/* Calcula el índice de la dirección donde se almacena la direccións
	base del operand a la que se sumará el desplazamiento Y */
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

ASL::ASL(int operand, CPU* cpu) : Instruction(operand, cpu) {}

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
ASL_accumulator::ASL_accumulator(int operand, CPU* cpu) : ASL(operand, cpu) {}

int ASL_accumulator::execute() {
	int result = ASL::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ASL_zero
ASL_zero::ASL_zero(int operand, CPU* cpu) : ASL(operand, cpu) {}

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
ASL_zerox::ASL_zerox(int operand, CPU* cpu) : ASL(operand, cpu) {}

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
ASL_abs::ASL_abs(int operand, CPU* cpu) : ASL(operand, cpu) {}

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
ASL_absx::ASL_absx(int operand, CPU* cpu) : ASL(operand, cpu) {}

int ASL_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * BCC Branch on Carry Clear
 *****************************************************************************/

BCC::BCC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BCC::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (!cpu->getReg_p_c_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BCS Branch on carry set
 *****************************************************************************/

BCS::BCS(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BCS::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (cpu->getReg_p_c_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BEQ Branch on result zero
 *****************************************************************************/

BEQ::BEQ(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BEQ::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (cpu->getReg_p_z_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BIT Test bits in memory with accumulator
 *****************************************************************************/

BIT::BIT(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BIT::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);

	// Transfiere el bit de Signo
	if (op & 0x80)
		cpu->setReg_p_s_bit(1);
	else
		cpu->setReg_p_s_bit(0);

	// Transfiere el bit de Overflow
	if (op & 0x40)
		cpu->setReg_p_v_bit(1);
	else
		cpu->setReg_p_v_bit(0);

	// Calcula el bit Zero
	if (!(op & cpu->getRegA()))
		cpu->setReg_p_z_bit(1);
	else
		cpu->setReg_p_z_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


BIT_zero::BIT_zero(int operand, CPU* cpu) : BIT(operand, cpu) {}

BIT_abs::BIT_abs(int operand, CPU* cpu) : BIT(operand, cpu) {}


/******************************************************************************
 * BMI Branch on result minus
 *****************************************************************************/

BMI::BMI(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BMI::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (cpu->getReg_p_s_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BNE Branch on result not zero
 *****************************************************************************/

BNE::BNE(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BNE::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (!cpu->getReg_p_z_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BPL Branch on result plus
 *****************************************************************************/

BPL::BPL(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BPL::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (!cpu->getReg_p_s_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BRK Force Break
 *****************************************************************************/

BRK::BRK(CPU* cpu) : Instruction(0, cpu) {}

int BRK::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	cpu->incrPc(BYTES);
	int pc = cpu->getRegPc() + 1;
	cpu->pushStack((pc >> 8) & 0xFF);
	cpu->pushStack(pc & 0xFF);
	cpu->setReg_p_b_bit(1);

	// Los bit 4 y 5 se ponen siempre a 1
	cpu->pushStack(cpu->getRegP() | 0x30);
	cpu->setReg_p_i_bit(1);

	pc = cpu->getMem()->readData(0xFFFE);
	pc = pc | (cpu->getMem()->readData(0xFFFF) << 8);
	cpu->setRegPc(pc);

	return CYCLES;
}


/******************************************************************************
 * BVC Branch on overflow clear
 *****************************************************************************/

BVC::BVC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BVC::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (!cpu->getReg_p_v_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * BVS Branch on overflow set
 *****************************************************************************/

BVS::BVS(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int BVS::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	if (cpu->getReg_p_v_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return CYCLES;
}


/******************************************************************************
 * CLC Clear carry flag
 *****************************************************************************/

CLC::CLC(CPU* cpu) : Instruction(0, cpu) {}

int CLC::execute() {
	cpu->setReg_p_c_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * CLD Clear decimal mode
 *****************************************************************************/

CLD::CLD(CPU* cpu) : Instruction(0, cpu) {}

int CLD::execute() {
	cpu->setReg_p_d_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * CLI Clear interrupt disable bit
 *****************************************************************************/

CLI::CLI(CPU* cpu) : Instruction(0, cpu) {}

int CLI::execute() {
	cpu->setReg_p_i_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * CLV Clear overflow flag
 *****************************************************************************/

CLV::CLV(CPU* cpu) : Instruction(0, cpu) {}

int CLV::execute() {
	cpu->setReg_p_v_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * CMP Compare memory and accumulator
 *****************************************************************************/

CMP::CMP(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int CMP::execute(int op) {
	int ac = cpu->getRegA();
	int result = ac - op;

	if (0 <= result < 0x100)
		cpu->setReg_p_c_bit(1);
	else
		cpu->setReg_p_c_bit(0);

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// CMP_inmediate
CMP_inmediate::CMP_inmediate(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_inmediate::execute() {
	return CMP::execute(operand);
}


// CMP_zero
CMP_zero::CMP_zero(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_zerox
CMP_zerox::CMP_zerox(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_abs
CMP_abs::CMP_abs(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_absx
CMP_absx::CMP_absx(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_absy
CMP_absy::CMP_absy(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_preindexi
CMP_preindexi::CMP_preindexi(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_postindexi
CMP_postindexi::CMP_postindexi(int operand, CPU* cpu) : CMP(operand, cpu) {}

int CMP_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


/******************************************************************************
 * CPX Compare Memory and Index X
 *****************************************************************************/

CPX::CPX(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int CPX::execute(int op) {
	int reg_x = cpu->getRegX();
	int result = reg_x - op;

	if (0 <= result < 0x100)
		cpu->setReg_p_c_bit(1);
	else
		cpu->setReg_p_c_bit(0);

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// CPX_inmediate
CPX_inmediate::CPX_inmediate(int operand, CPU* cpu) : CPX(operand, cpu) {}

int CPX_inmediate::execute() {
	return CPX::execute(operand);
}


// CPX_zero
CPX_zero::CPX_zero(int operand, CPU* cpu) : CPX(operand, cpu) {}

int CPX_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPX::execute(op);
}


// CPX_abs
CPX_abs::CPX_abs(int operand, CPU* cpu) : CPX(operand, cpu) {}

int CPX_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPX::execute(op);
}


/******************************************************************************
 * CPY Compare Memory and Index Y
 *****************************************************************************/

CPY::CPY(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int CPY::execute(int op) {
	int reg_y = cpu->getRegY();
	int result = reg_y - op;

	if (0 <= result < 0x100)
		cpu->setReg_p_c_bit(1);
	else
		cpu->setReg_p_c_bit(0);

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// CPY_inmediate
CPY_inmediate::CPY_inmediate(int operand, CPU* cpu) : CPY(operand, cpu) {}

int CPY_inmediate::execute() {
	return CPY::execute(operand);
}


// CPY_zero
CPY_zero::CPY_zero(int operand, CPU* cpu) : CPY(operand, cpu) {}

int CPY_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPY::execute(op);
}


// CPY_abs
CPY_abs::CPY_abs(int operand, CPU* cpu) : CPY(operand, cpu) {}

int CPY_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPY::execute(op);
}


/******************************************************************************
 * DEC: Shift Left One Bit (Memory or Accumulator)
 *****************************************************************************/

DEC::DEC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int DEC::execute(int op) {
	int result = (op - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// DEC_zero
DEC_zero::DEC_zero(int operand, CPU* cpu) : DEC(operand, cpu) {}

int DEC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// DEC_zerox
DEC_zerox::DEC_zerox(int operand, CPU* cpu) : DEC(operand, cpu) {}

int DEC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// DEC_abs
DEC_abs::DEC_abs(int operand, CPU* cpu) : DEC(operand, cpu) {}

int DEC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// DEC_absx
DEC_absx::DEC_absx(int operand, CPU* cpu) : DEC(operand, cpu) {}

int DEC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * DEX Decrement index X by one
 *****************************************************************************/

DEX::DEX(CPU* cpu) : Instruction(0, cpu) {}

int DEX::execute() {
	int result = (cpu->getRegX() - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegX(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * DEY Decrement index Y by one
 *****************************************************************************/

DEY::DEY(CPU* cpu) : Instruction(0, cpu) {}

int DEY::execute() {
	int result = (cpu->getRegY() - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegY(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * EOR "Exclusive-Or" memory with accumulator
 *****************************************************************************/

EOR::EOR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int EOR::execute(int op) {
	int ac = cpu->getRegA();
	int result = ac ^ op;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// EOR_inmediate
EOR_inmediate::EOR_inmediate(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_inmediate::execute() {
	return EOR::execute(operand);
}


// EOR_zero
EOR_zero::EOR_zero(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_zerox
EOR_zerox::EOR_zerox(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_abs
EOR_abs::EOR_abs(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_absx
EOR_absx::EOR_absx(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_absy
EOR_absy::EOR_absy(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_preindexi
EOR_preindexi::EOR_preindexi(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_postindexi
EOR_postindexi::EOR_postindexi(int operand, CPU* cpu) : EOR(operand, cpu) {}

int EOR_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


/******************************************************************************
 * INC Increment memory by one
 *****************************************************************************/

INC::INC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int INC::execute(int op) {
	int result = (op + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// INC_zero
INC_zero::INC_zero(int operand, CPU* cpu) : INC(operand, cpu) {}

int INC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// INC_zerox
INC_zerox::INC_zerox(int operand, CPU* cpu) : INC(operand, cpu) {}

int INC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// INC_abs
INC_abs::INC_abs(int operand, CPU* cpu) : INC(operand, cpu) {}

int INC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// INC_absx
INC_absx::INC_absx(int operand, CPU* cpu) : INC(operand, cpu) {}

int INC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * INX Increment Index X by one
 *****************************************************************************/

INX::INX(CPU* cpu) : Instruction(0, cpu) {}

int INX::execute() {
	int result = (cpu->getRegX() + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegX(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * INY Increment Index Y by one
 *****************************************************************************/

INY::INY(CPU* cpu) : Instruction(0, cpu) {}

int INY::execute() {
	int result = (cpu->getRegY() + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegY(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * JMP Jump to new location
 *****************************************************************************/

JMP::JMP(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int JMP::execute(int op) {
	cpu->setRegPc(op);
    return CYCLES;
}


// JMP_abs
JMP_abs::JMP_abs(int operand, CPU* cpu) : JMP(operand, cpu) {}

int JMP_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	JMP::execute(addr);

	return CYCLES;
}


// JMP_indirect
JMP_indirect::JMP_indirect(int operand, CPU* cpu) : JMP(operand, cpu) {}

int JMP_indirect::execute() {
	Memory* mem = cpu->getMem();

	int addr = mem->readData(operand);

	if (operand & 0xFF == 0xFF)
		addr = addr | (mem->readData(operand & 0xFF00) << 8);
	else
		addr = addr | (mem->readData(operand + 1) << 8);

	JMP::execute(addr);

	return CYCLES;
}


/******************************************************************************
 * JSR Jump to new location saving return address
 *****************************************************************************/

JSR::JSR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int JSR::execute(int op) {
	int pc = cpu->getRegPc() + BYTES - 1;
	cpu->pushStack((pc >> 8) & 0xFF);
	cpu->pushStack(pc & 0xFF);

	cpu->setRegPc(operand);

	return CYCLES;
}


/******************************************************************************
 * LDA Load accumulator with memory
 *****************************************************************************/

LDA::LDA(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int LDA::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegA(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LDA_inmediate
LDA_inmediate::LDA_inmediate(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_inmediate::execute() {
	return LDA::execute(operand);
}


// LDA_zero
LDA_zero::LDA_zero(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_zerox
LDA_zerox::LDA_zerox(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_abs
LDA_abs::LDA_abs(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_absx
LDA_absx::LDA_absx(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_absy
LDA_absy::LDA_absy(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_preindexi
LDA_preindexi::LDA_preindexi(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_postindexi
LDA_postindexi::LDA_postindexi(int operand, CPU* cpu) : LDA(operand, cpu) {}

int LDA_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


/******************************************************************************
 * LDX Load index X with memory
 *****************************************************************************/

LDX::LDX(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int LDX::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegX(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LDX_inmediate
LDX_inmediate::LDX_inmediate(int operand, CPU* cpu) : LDX(operand, cpu) {}

int LDX_inmediate::execute() {
	return LDX::execute(operand);
}


// LDX_zero
LDX_zero::LDX_zero(int operand, CPU* cpu) : LDX(operand, cpu) {}

int LDX_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_zerox
LDX_zeroy::LDX_zeroy(int operand, CPU* cpu) : LDX(operand, cpu) {}

int LDX_zeroy::execute() {
	int addr = fetchIndexedZeroYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_abs
LDX_abs::LDX_abs(int operand, CPU* cpu) : LDX(operand, cpu) {}

int LDX_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_absy
LDX_absy::LDX_absy(int operand, CPU* cpu) : LDX(operand, cpu) {}

int LDX_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


/******************************************************************************
 * LDY Load index Y with memory
 *****************************************************************************/

LDY::LDY(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int LDY::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegY(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LDY_inmediate
LDY_inmediate::LDY_inmediate(int operand, CPU* cpu) : LDY(operand, cpu) {}

int LDY_inmediate::execute() {
	return LDY::execute(operand);
}


// LDY_zero
LDY_zero::LDY_zero(int operand, CPU* cpu) : LDY(operand, cpu) {}

int LDY_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_zerox
LDY_zerox::LDY_zerox(int operand, CPU* cpu) : LDY(operand, cpu) {}

int LDY_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_abs
LDY_abs::LDY_abs(int operand, CPU* cpu) : LDY(operand, cpu) {}

int LDY_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_absy
LDY_absx::LDY_absx(int operand, CPU* cpu) : LDY(operand, cpu) {}

int LDY_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


/******************************************************************************
 * LSR Shift right one bit (memory or accumulator)
 *****************************************************************************/

LSR::LSR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int LSR::execute(int op) {
	int result = op >> 1;

	cpu->setReg_p_c_bit(op & 0x01);
	cpu->setReg_p_s_bit(0);
	cpu->setZeroBit(result);

	return result & 0xFF;
}


// LSR_accumulator
LSR_accumulator::LSR_accumulator(int operand, CPU* cpu) : LSR(operand, cpu) {}

int LSR_accumulator::execute() {
	int result = LSR::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LSR_zero
LSR_zero::LSR_zero(int operand, CPU* cpu) : LSR(operand, cpu) {}

int LSR_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LSR_zerox
LSR_zerox::LSR_zerox(int operand, CPU* cpu) : LSR(operand, cpu) {}

int LSR_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LSR_abs
LSR_abs::LSR_abs(int operand, CPU* cpu) : LSR(operand, cpu) {}

int LSR_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// LSR_absx
LSR_absx::LSR_absx(int operand, CPU* cpu) : LSR(operand, cpu) {}

int LSR_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * NOP No operation
 *****************************************************************************/

NOP::NOP(CPU* cpu) : Instruction(0, cpu) {}

int NOP::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


/******************************************************************************
 * ORA "OR" memory with accumulator
 *****************************************************************************/

ORA::ORA(int operand, CPU* cpu) : Instruction(operand, cpu) {}

int ORA::execute(int op) {
	int ac = cpu->getRegA();
	int result = ac | op;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(BYTES);

	return CYCLES;
}


// ORA_inmediate
ORA_inmediate::ORA_inmediate(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_inmediate::execute() {
	return ORA::execute(operand);
}


// ORA_zero
ORA_zero::ORA_zero(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_zerox
ORA_zerox::ORA_zerox(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_abs
ORA_abs::ORA_abs(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_absx
ORA_absx::ORA_absx(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_absy
ORA_absy::ORA_absy(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_preindexi
ORA_preindexi::ORA_preindexi(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_postindexi
ORA_postindexi::ORA_postindexi(int operand, CPU* cpu) : ORA(operand, cpu) {}

int ORA_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}



