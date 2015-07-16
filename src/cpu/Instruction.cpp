/*
 * Instruction.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "Instruction.hpp"

#include <stddef.h>

#include "../Memory.hpp"
#include "../nesutils.hpp"
#include "CPU.hpp"


Instruction::Instruction(int operand, CPU* cpu) {
	this->operand = operand;
	this->cpu = cpu;
}


Instruction::~Instruction() {
}


int Instruction::getOperand() {
	return operand;
}


void Instruction::setOperand(int op) {
	operand = op;
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

int Instruction::execute() {
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

ADC::~ADC() {
}

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
	cpu->incrPc(getBytes());

	return getCycles();
}


// ADC_inmediate
ADC_inmediate::ADC_inmediate(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_inmediate::~ADC_inmediate() {
}

int ADC_inmediate::execute() {
	return ADC::execute(operand);
}


// ADC_zero
ADC_zero::ADC_zero(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_zero::~ADC_zero() {
}

int ADC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_zerox
ADC_zerox::ADC_zerox(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_zerox::~ADC_zerox() {
}

int ADC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_abs
ADC_abs::ADC_abs(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_abs::~ADC_abs() {
}

int ADC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_absx
ADC_absx::ADC_absx(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_absx::~ADC_absx() {
}

int ADC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_absy
ADC_absy::ADC_absy(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_absy::~ADC_absy() {
}

int ADC_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_preindexi
ADC_preindexi::ADC_preindexi(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_preindexi::~ADC_preindexi() {
}

int ADC_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


// ADC_postindexi
ADC_postindexi::ADC_postindexi(int operand, CPU* cpu) : ADC(operand, cpu) {}

ADC_postindexi::~ADC_postindexi() {
}

int ADC_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ADC::execute(op);
}


/******************************************************************************
 * AND: And memory with accumulator
 *****************************************************************************/

AND::AND(int operand, CPU* cpu) : Instruction(operand, cpu) {}

AND::~AND() {
}

int AND::execute(int op) {
	int ac = cpu->getRegA();
	int rst = ac & op;

	// Establece el bit SIGN del registro P
	cpu->setSignBit(rst);
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(rst);

	cpu->setRegA(rst);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// AND_inmediate
AND_inmediate::AND_inmediate(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_inmediate::~AND_inmediate() {
}

int AND_inmediate::execute() {
	return AND::execute(operand);
}


// AND_zero
AND_zero::AND_zero(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_zero::~AND_zero() {
}

int AND_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_zerox
AND_zerox::AND_zerox(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_zerox::~AND_zerox() {
}

int AND_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_abs
AND_abs::AND_abs(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_abs::~AND_abs() {
}

int AND_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_absx
AND_absx::AND_absx(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_absx::~AND_absx() {
}

int AND_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_absy
AND_absy::AND_absy(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_absy::~AND_absy() {
}

int AND_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_preindexi
AND_preindexi::AND_preindexi(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_preindexi::~AND_preindexi() {
}

int AND_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


// AND_postindexi
AND_postindexi::AND_postindexi(int operand, CPU* cpu) : AND(operand, cpu) {}

AND_postindexi::~AND_postindexi() {
}

int AND_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return AND::execute(op);
}


/******************************************************************************
 * ASL: Shift Left One Bit (Memory or Accumulator)
 *****************************************************************************/

ASL::ASL(int operand, CPU* cpu) : Instruction(operand, cpu) {}

ASL::~ASL() {
}

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

ASL_accumulator::~ASL_accumulator() {
}

int ASL_accumulator::execute() {
	int result = ASL::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ASL_zero
ASL_zero::ASL_zero(int operand, CPU* cpu) : ASL(operand, cpu) {}

ASL_zero::~ASL_zero() {
}

int ASL_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ASL_zerox
ASL_zerox::ASL_zerox(int operand, CPU* cpu) : ASL(operand, cpu) {}

ASL_zerox::~ASL_zerox() {
}

int ASL_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ASL_abs
ASL_abs::ASL_abs(int operand, CPU* cpu) : ASL(operand, cpu) {}

ASL_abs::~ASL_abs() {
}

int ASL_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ASL_absx
ASL_absx::ASL_absx(int operand, CPU* cpu) : ASL(operand, cpu) {}

ASL_absx::~ASL_absx() {
}

int ASL_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ASL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * BCC Branch on Carry Clear
 *****************************************************************************/

BCC::BCC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BCC::~BCC() {
}

int BCC::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (!cpu->getReg_p_c_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BCS Branch on carry set
 *****************************************************************************/

BCS::BCS(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BCS::~BCS() {
}

int BCS::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (cpu->getReg_p_c_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BEQ Branch on result zero
 *****************************************************************************/

BEQ::BEQ(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BEQ::~BEQ() {
}

int BEQ::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (cpu->getReg_p_z_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BIT Test bits in memory with accumulator
 *****************************************************************************/

BIT::BIT(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BIT::~BIT() {
}

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
	cpu->incrPc(getBytes());

	return getCycles();
}


BIT_zero::BIT_zero(int operand, CPU* cpu) : BIT(operand, cpu) {}


BIT_zero::~BIT_zero() {
}


BIT_abs::BIT_abs(int operand, CPU* cpu) : BIT(operand, cpu) {}


BIT_abs::~BIT_abs() {
}


/******************************************************************************
 * BMI Branch on result minus
 *****************************************************************************/

BMI::BMI(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BMI::~BMI() {
}

int BMI::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (cpu->getReg_p_s_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BNE Branch on result not zero
 *****************************************************************************/

BNE::BNE(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BNE::~BNE() {
}

int BNE::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (!cpu->getReg_p_z_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BPL Branch on result plus
 *****************************************************************************/

BPL::BPL(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BPL::~BPL() {
}

int BPL::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (!cpu->getReg_p_s_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BRK Force Break
 *****************************************************************************/

BRK::BRK(CPU* cpu) : Instruction(0, cpu) {}

BRK::~BRK() {
}

int BRK::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	cpu->incrPc(getBytes());
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

	return getCycles();
}


/******************************************************************************
 * BVC Branch on overflow clear
 *****************************************************************************/

BVC::BVC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BVC::~BVC() {
}

int BVC::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (!cpu->getReg_p_v_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * BVS Branch on overflow set
 *****************************************************************************/

BVS::BVS(int operand, CPU* cpu) : Instruction(operand, cpu) {}

BVS::~BVS() {
}

int BVS::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (cpu->getReg_p_v_bit())
		cpu->setRegPc(cpu->getRegPc() + c2ToInt(operand));

	return getCycles();
}


/******************************************************************************
 * CLC Clear carry flag
 *****************************************************************************/

CLC::CLC(CPU* cpu) : Instruction(0, cpu) {}

CLC::~CLC() {
}

int CLC::execute() {
	cpu->setReg_p_c_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * CLD Clear decimal mode
 *****************************************************************************/

CLD::CLD(CPU* cpu) : Instruction(0, cpu) {}

CLD::~CLD() {
}

int CLD::execute() {
	cpu->setReg_p_d_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * CLI Clear interrupt disable bit
 *****************************************************************************/

CLI::CLI(CPU* cpu) : Instruction(0, cpu) {}

CLI::~CLI() {
}

int CLI::execute() {
	cpu->setReg_p_i_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * CLV Clear overflow flag
 *****************************************************************************/

CLV::CLV(CPU* cpu) : Instruction(0, cpu) {}

CLV::~CLV() {
}

int CLV::execute() {
	cpu->setReg_p_v_bit(0);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * CMP Compare memory and accumulator
 *****************************************************************************/

CMP::CMP(int operand, CPU* cpu) : Instruction(operand, cpu) {}

CMP::~CMP() {
}

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
	cpu->incrPc(getBytes());

	return getCycles();
}


// CMP_inmediate
CMP_inmediate::CMP_inmediate(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_inmediate::~CMP_inmediate() {
}

int CMP_inmediate::execute() {
	return CMP::execute(operand);
}


// CMP_zero
CMP_zero::CMP_zero(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_zero::~CMP_zero() {
}

int CMP_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_zerox
CMP_zerox::CMP_zerox(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_zerox::~CMP_zerox() {
}

int CMP_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_abs
CMP_abs::CMP_abs(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_abs::~CMP_abs() {
}

int CMP_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_absx
CMP_absx::CMP_absx(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_absx::~CMP_absx() {
}

int CMP_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_absy
CMP_absy::CMP_absy(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_absy::~CMP_absy() {
}

int CMP_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_preindexi
CMP_preindexi::CMP_preindexi(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_preindexi::~CMP_preindexi() {
}

int CMP_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


// CMP_postindexi
CMP_postindexi::CMP_postindexi(int operand, CPU* cpu) : CMP(operand, cpu) {}

CMP_postindexi::~CMP_postindexi() {
}

int CMP_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CMP::execute(op);
}


/******************************************************************************
 * CPX Compare Memory and Index X
 *****************************************************************************/

CPX::CPX(int operand, CPU* cpu) : Instruction(operand, cpu) {}

CPX::~CPX() {
}

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
	cpu->incrPc(getBytes());

	return getCycles();
}


// CPX_inmediate
CPX_inmediate::CPX_inmediate(int operand, CPU* cpu) : CPX(operand, cpu) {}

CPX_inmediate::~CPX_inmediate() {
}

int CPX_inmediate::execute() {
	return CPX::execute(operand);
}


// CPX_zero
CPX_zero::CPX_zero(int operand, CPU* cpu) : CPX(operand, cpu) {}

CPX_zero::~CPX_zero() {
}

int CPX_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPX::execute(op);
}


// CPX_abs
CPX_abs::CPX_abs(int operand, CPU* cpu) : CPX(operand, cpu) {}

CPX_abs::~CPX_abs() {
}

int CPX_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPX::execute(op);
}


/******************************************************************************
 * CPY Compare Memory and Index Y
 *****************************************************************************/

CPY::CPY(int operand, CPU* cpu) : Instruction(operand, cpu) {}

CPY::~CPY() {
}

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
	cpu->incrPc(getBytes());

	return getCycles();
}


// CPY_inmediate
CPY_inmediate::CPY_inmediate(int operand, CPU* cpu) : CPY(operand, cpu) {}

CPY_inmediate::~CPY_inmediate() {
}

int CPY_inmediate::execute() {
	return CPY::execute(operand);
}


// CPY_zero
CPY_zero::CPY_zero(int operand, CPU* cpu) : CPY(operand, cpu) {}

CPY_zero::~CPY_zero() {
}

int CPY_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPY::execute(op);
}


// CPY_abs
CPY_abs::CPY_abs(int operand, CPU* cpu) : CPY(operand, cpu) {}

CPY_abs::~CPY_abs() {
}

int CPY_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return CPY::execute(op);
}


/******************************************************************************
 * DEC: Shift Left One Bit (Memory or Accumulator)
 *****************************************************************************/

DEC::DEC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

DEC::~DEC() {
}

int DEC::execute(int op) {
	int result = (op - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// DEC_zero
DEC_zero::DEC_zero(int operand, CPU* cpu) : DEC(operand, cpu) {}

DEC_zero::~DEC_zero() {
}

int DEC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// DEC_zerox
DEC_zerox::DEC_zerox(int operand, CPU* cpu) : DEC(operand, cpu) {}

DEC_zerox::~DEC_zerox() {
}

int DEC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// DEC_abs
DEC_abs::DEC_abs(int operand, CPU* cpu) : DEC(operand, cpu) {}

DEC_abs::~DEC_abs() {
}

int DEC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// DEC_absx
DEC_absx::DEC_absx(int operand, CPU* cpu) : DEC(operand, cpu) {}

DEC_absx::~DEC_absx() {
}

int DEC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = DEC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * DEX Decrement index X by one
 *****************************************************************************/

DEX::DEX(CPU* cpu) : Instruction(0, cpu) {}

DEX::~DEX() {
}

int DEX::execute() {
	int result = (cpu->getRegX() - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegX(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * DEY Decrement index Y by one
 *****************************************************************************/

DEY::DEY(CPU* cpu) : Instruction(0, cpu) {}

DEY::~DEY() {
}

int DEY::execute() {
	int result = (cpu->getRegY() - 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegY(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * EOR "Exclusive-Or" memory with accumulator
 *****************************************************************************/

EOR::EOR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

EOR::~EOR() {
}

int EOR::execute(int op) {
	int ac = cpu->getRegA();
	int result = ac ^ op;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// EOR_inmediate
EOR_inmediate::EOR_inmediate(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_inmediate::~EOR_inmediate() {
}

int EOR_inmediate::execute() {
	return EOR::execute(operand);
}


// EOR_zero
EOR_zero::EOR_zero(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_zero::~EOR_zero() {
}

int EOR_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_zerox
EOR_zerox::EOR_zerox(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_zerox::~EOR_zerox() {
}

int EOR_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_abs
EOR_abs::EOR_abs(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_abs::~EOR_abs() {
}

int EOR_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_absx
EOR_absx::EOR_absx(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_absx::~EOR_absx() {
}

int EOR_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_absy
EOR_absy::EOR_absy(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_absy::~EOR_absy() {
}

int EOR_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_preindexi
EOR_preindexi::EOR_preindexi(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_preindexi::~EOR_preindexi() {
}

int EOR_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


// EOR_postindexi
EOR_postindexi::EOR_postindexi(int operand, CPU* cpu) : EOR(operand, cpu) {}

EOR_postindexi::~EOR_postindexi() {
}

int EOR_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return EOR::execute(op);
}


/******************************************************************************
 * INC Increment memory by one
 *****************************************************************************/

INC::INC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

INC::~INC() {
}

int INC::execute(int op) {
	int result = (op + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// INC_zero
INC_zero::INC_zero(int operand, CPU* cpu) : INC(operand, cpu) {}

INC_zero::~INC_zero() {
}

int INC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// INC_zerox
INC_zerox::INC_zerox(int operand, CPU* cpu) : INC(operand, cpu) {}

INC_zerox::~INC_zerox() {
}

int INC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// INC_abs
INC_abs::INC_abs(int operand, CPU* cpu) : INC(operand, cpu) {}

INC_abs::~INC_abs() {
}

int INC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// INC_absx
INC_absx::INC_absx(int operand, CPU* cpu) : INC(operand, cpu) {}

INC_absx::~INC_absx() {
}

int INC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = INC::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * INX Increment Index X by one
 *****************************************************************************/

INX::INX(CPU* cpu) : Instruction(0, cpu) {}

INX::~INX() {
}

int INX::execute() {
	int result = (cpu->getRegX() + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegX(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * INY Increment Index Y by one
 *****************************************************************************/

INY::INY(CPU* cpu) : Instruction(0, cpu) {}

INY::~INY() {
}

int INY::execute() {
	int result = (cpu->getRegY() + 1) & 0xFF;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegY(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * JMP Jump to new location
 *****************************************************************************/

JMP::JMP(int operand, CPU* cpu) : Instruction(operand, cpu) {}

JMP::~JMP() {
}

int JMP::execute(int op) {
	cpu->setRegPc(op);
    return getCycles();
}


// JMP_abs
JMP_abs::JMP_abs(int operand, CPU* cpu) : JMP(operand, cpu) {}

JMP_abs::~JMP_abs() {
}

int JMP_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	JMP::execute(addr);

	return getCycles();
}


// JMP_indirect
JMP_indirect::JMP_indirect(int operand, CPU* cpu) : JMP(operand, cpu) {}

JMP_indirect::~JMP_indirect() {
}

int JMP_indirect::execute() {
	Memory* mem = cpu->getMem();

	int addr = mem->readData(operand);

	if (operand & 0xFF == 0xFF)
		addr = addr | (mem->readData(operand & 0xFF00) << 8);
	else
		addr = addr | (mem->readData(operand + 1) << 8);

	JMP::execute(addr);

	return getCycles();
}


/******************************************************************************
 * JSR Jump to new location saving return address
 *****************************************************************************/

JSR::JSR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

JSR::~JSR() {
}

int JSR::execute(int op) {
	int pc = cpu->getRegPc() + getBytes() - 1;
	cpu->pushStack((pc >> 8) & 0xFF);
	cpu->pushStack(pc & 0xFF);

	cpu->setRegPc(operand);

	return getCycles();
}


/******************************************************************************
 * LDA Load accumulator with memory
 *****************************************************************************/

LDA::LDA(int operand, CPU* cpu) : Instruction(operand, cpu) {}

LDA::~LDA() {
}

int LDA::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegA(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LDA_inmediate
LDA_inmediate::LDA_inmediate(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_inmediate::~LDA_inmediate() {
}

int LDA_inmediate::execute() {
	return LDA::execute(operand);
}


// LDA_zero
LDA_zero::LDA_zero(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_zero::~LDA_zero() {
}

int LDA_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_zerox
LDA_zerox::LDA_zerox(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_zerox::~LDA_zerox() {
}

int LDA_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_abs
LDA_abs::LDA_abs(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_abs::~LDA_abs() {
}

int LDA_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_absx
LDA_absx::LDA_absx(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_absx::~LDA_absx() {
}

int LDA_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_absy
LDA_absy::LDA_absy(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_absy::~LDA_absy() {
}

int LDA_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_preindexi
LDA_preindexi::LDA_preindexi(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_preindexi::~LDA_preindexi() {
}

int LDA_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


// LDA_postindexi
LDA_postindexi::LDA_postindexi(int operand, CPU* cpu) : LDA(operand, cpu) {}

LDA_postindexi::~LDA_postindexi() {
}

int LDA_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDA::execute(op);
}


/******************************************************************************
 * LDX Load index X with memory
 *****************************************************************************/

LDX::LDX(int operand, CPU* cpu) : Instruction(operand, cpu) {}

LDX::~LDX() {
}

int LDX::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegX(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LDX_inmediate
LDX_inmediate::LDX_inmediate(int operand, CPU* cpu) : LDX(operand, cpu) {}

LDX_inmediate::~LDX_inmediate() {
}

int LDX_inmediate::execute() {
	return LDX::execute(operand);
}


// LDX_zero
LDX_zero::LDX_zero(int operand, CPU* cpu) : LDX(operand, cpu) {}

LDX_zero::~LDX_zero() {
}

int LDX_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_zerox
LDX_zeroy::LDX_zeroy(int operand, CPU* cpu) : LDX(operand, cpu) {}

LDX_zeroy::~LDX_zeroy() {
}

int LDX_zeroy::execute() {
	int addr = fetchIndexedZeroYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_abs
LDX_abs::LDX_abs(int operand, CPU* cpu) : LDX(operand, cpu) {}

LDX_abs::~LDX_abs() {
}

int LDX_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


// LDX_absy
LDX_absy::LDX_absy(int operand, CPU* cpu) : LDX(operand, cpu) {}

LDX_absy::~LDX_absy() {
}

int LDX_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDX::execute(op);
}


/******************************************************************************
 * LDY Load index Y with memory
 *****************************************************************************/

LDY::LDY(int operand, CPU* cpu) : Instruction(operand, cpu) {}

LDY::~LDY() {
}

int LDY::execute(int op) {
	// Establece el bit ZERO del registro P
	cpu->setZeroBit(op);
	// Establece el bit SIGN del registro P
	cpu->setSignBit(op);

	cpu->setRegY(op);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LDY_inmediate
LDY_inmediate::LDY_inmediate(int operand, CPU* cpu) : LDY(operand, cpu) {}

LDY_inmediate::~LDY_inmediate() {
}

int LDY_inmediate::execute() {
	return LDY::execute(operand);
}


// LDY_zero
LDY_zero::LDY_zero(int operand, CPU* cpu) : LDY(operand, cpu) {}

LDY_zero::~LDY_zero() {
}

int LDY_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_zerox
LDY_zerox::LDY_zerox(int operand, CPU* cpu) : LDY(operand, cpu) {}

LDY_zerox::~LDY_zerox() {
}

int LDY_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_abs
LDY_abs::LDY_abs(int operand, CPU* cpu) : LDY(operand, cpu) {}

LDY_abs::~LDY_abs() {
}

int LDY_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


// LDY_absy
LDY_absx::LDY_absx(int operand, CPU* cpu) : LDY(operand, cpu) {}

LDY_absx::~LDY_absx() {
}

int LDY_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return LDY::execute(op);
}


/******************************************************************************
 * LSR Shift right one bit (memory or accumulator)
 *****************************************************************************/

LSR::LSR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

LSR::~LSR() {
}

int LSR::execute(int op) {
	int result = op >> 1;

	cpu->setReg_p_c_bit(op & 0x01);
	cpu->setReg_p_s_bit(0);
	cpu->setZeroBit(result);

	return result & 0xFF;
}


// LSR_accumulator
LSR_accumulator::LSR_accumulator(int operand, CPU* cpu) : LSR(operand, cpu) {}

LSR_accumulator::~LSR_accumulator() {
}

int LSR_accumulator::execute() {
	int result = LSR::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LSR_zero
LSR_zero::LSR_zero(int operand, CPU* cpu) : LSR(operand, cpu) {}

LSR_zero::~LSR_zero() {
}

int LSR_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LSR_zerox
LSR_zerox::LSR_zerox(int operand, CPU* cpu) : LSR(operand, cpu) {}

LSR_zerox::~LSR_zerox() {
}

int LSR_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LSR_abs
LSR_abs::LSR_abs(int operand, CPU* cpu) : LSR(operand, cpu) {}

LSR_abs::~LSR_abs() {
}

int LSR_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// LSR_absx
LSR_absx::LSR_absx(int operand, CPU* cpu) : LSR(operand, cpu) {}

LSR_absx::~LSR_absx() {
}

int LSR_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = LSR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * NOP No operation
 *****************************************************************************/

NOP::NOP(CPU* cpu) : Instruction(0, cpu) {}

NOP::~NOP() {
}

int NOP::execute() {
	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * ORA "OR" memory with accumulator
 *****************************************************************************/

ORA::ORA(int operand, CPU* cpu) : Instruction(operand, cpu) {}

ORA::~ORA() {
}

int ORA::execute(int op) {
	int ac = cpu->getRegA();
	int result = ac | op;

	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ORA_inmediate
ORA_inmediate::ORA_inmediate(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_inmediate::~ORA_inmediate() {
}

int ORA_inmediate::execute() {
	return ORA::execute(operand);
}


// ORA_zero
ORA_zero::ORA_zero(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_zero::~ORA_zero() {
}

int ORA_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_zerox
ORA_zerox::ORA_zerox(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_zerox::~ORA_zerox() {
}

int ORA_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_abs
ORA_abs::ORA_abs(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_abs::~ORA_abs() {
}

int ORA_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_absx
ORA_absx::ORA_absx(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_absx::~ORA_absx() {
}

int ORA_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_absy
ORA_absy::ORA_absy(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_absy::~ORA_absy() {
}

int ORA_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_preindexi
ORA_preindexi::ORA_preindexi(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_preindexi::~ORA_preindexi() {
}

int ORA_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


// ORA_postindexi
ORA_postindexi::ORA_postindexi(int operand, CPU* cpu) : ORA(operand, cpu) {}

ORA_postindexi::~ORA_postindexi() {
}

int ORA_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return ORA::execute(op);
}


/******************************************************************************
 * PHA Push accumulator on stack
 *****************************************************************************/

PHA::PHA(CPU* cpu) : Instruction(0, cpu) {}

PHA::~PHA() {
}

int PHA::execute() {
	cpu->pushStack(cpu->getRegA());

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * PHP Push processor status on stack
 *****************************************************************************/

PHP::PHP(CPU* cpu) : Instruction(0, cpu) {}

PHP::~PHP() {
}

int PHP::execute() {
	// Los bit 4 y 5 se ponen siempre a 1
	cpu->pushStack(cpu->getRegP() | 0x30);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * PLA Pull accumulator from stack
 *****************************************************************************/

PLA::PLA(CPU* cpu) : Instruction(0, cpu) {}

PLA::~PLA() {
}

int PLA::execute() {
	int a = cpu->pullStack();
	cpu->setRegA(a);
	cpu->setSignBit(a);
	cpu->setZeroBit(a);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * PLP Pull processor status from stack
 *****************************************************************************/

PLP::PLP(CPU* cpu) : Instruction(0, cpu) {}

PLP::~PLP() {
}

int PLP::execute() {
	int p = cpu->pullStack();
	cpu->setRegP(p);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * ROL Rotate one bit left (memory or accumulator)
 *****************************************************************************/

ROL::ROL(int operand, CPU* cpu) : Instruction(operand, cpu) {}

ROL::~ROL() {
}

int ROL::execute(int op) {
	int result = op << 1;
	if (cpu->getReg_p_c_bit())
		result = result | 0x01;

	cpu->setCarryBit(result);
	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// ROL_accumulator
ROL_accumulator::ROL_accumulator(int operand, CPU* cpu) : ROL(operand, cpu) {}

ROL_accumulator::~ROL_accumulator() {
}

int ROL_accumulator::execute() {
	int result = ROL::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROL_zero
ROL_zero::ROL_zero(int operand, CPU* cpu) : ROL(operand, cpu) {}

ROL_zero::~ROL_zero() {
}

int ROL_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROL_zerox
ROL_zerox::ROL_zerox(int operand, CPU* cpu) : ROL(operand, cpu) {}

ROL_zerox::~ROL_zerox() {
}

int ROL_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROL_abs
ROL_abs::ROL_abs(int operand, CPU* cpu) : ROL(operand, cpu) {}

ROL_abs::~ROL_abs() {
}

int ROL_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROL_absx
ROL_absx::ROL_absx(int operand, CPU* cpu) : ROL(operand, cpu) {}

ROL_absx::~ROL_absx() {
}

int ROL_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROL::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * ROR Rotate one bit right (memory or accumulator)
 *****************************************************************************/

ROR::ROR(int operand, CPU* cpu) : Instruction(operand, cpu) {}

ROR::~ROR() {
}

int ROR::execute(int op) {
	int result = op >> 1;

	if (cpu->getReg_p_c_bit())
		result = result | 0x80;

	cpu->setReg_p_c_bit(op & 0x01);
	cpu->setSignBit(result);
	cpu->setZeroBit(result);

	return result;
}


// ROR_accumulator
ROR_accumulator::ROR_accumulator(int operand, CPU* cpu) : ROR(operand, cpu) {}

ROR_accumulator::~ROR_accumulator() {
}

int ROR_accumulator::execute() {
	int result = ROR::execute(cpu->getRegA());
	cpu->setRegA(result);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROR_zero
ROR_zero::ROR_zero(int operand, CPU* cpu) : ROR(operand, cpu) {}

ROR_zero::~ROR_zero() {
}

int ROR_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROR_zerox
ROR_zerox::ROR_zerox(int operand, CPU* cpu) : ROR(operand, cpu) {}

ROR_zerox::~ROR_zerox() {
}

int ROR_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROR_abs
ROR_abs::ROR_abs(int operand, CPU* cpu) : ROR(operand, cpu) {}

ROR_abs::~ROR_abs() {
}

int ROR_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// ROR_absx
ROR_absx::ROR_absx(int operand, CPU* cpu) : ROR(operand, cpu) {}

ROR_absx::~ROR_absx() {
}

int ROR_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	int result = ROR::execute(op);
	cpu->getMem()->writeData(result, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * RTI Return from interrupt
 *****************************************************************************/

RTI::RTI(CPU* cpu) : Instruction(0, cpu) {}

RTI::~RTI() {
}

int RTI::execute() {
	int p = cpu->pullStack();
	cpu->setRegP(p);

	int pc = cpu->pullStack();
	pc = pc | (cpu->pullStack() << 8);
	cpu->setRegPc(pc);

	return getCycles();
}


/******************************************************************************
 * RTS Return from subroutine
 *****************************************************************************/

RTS::RTS(CPU* cpu) : Instruction(0, cpu) {}

RTS::~RTS() {
}

int RTS::execute() {
	int pc = cpu->pullStack();
	pc = pc | (cpu->pullStack() << 8);
	pc = pc + 1;
	cpu->setRegPc(pc);

	return getCycles();
}


/******************************************************************************
 * SBC Subtract memory from accumulator with borrow
 *****************************************************************************/

SBC::SBC(int operand, CPU* cpu) : Instruction(operand, cpu) {}

SBC::~SBC() {
}

int SBC::execute(int op) {
	int ac = cpu->getRegA();
	int carry = cpu->getReg_p_c_bit();

	int rst = ac - op - 1 + carry;

	// Establece el bit CARRY del registro P
	if (0 <= rst < 0x100)
		cpu->setReg_p_c_bit(1);
	else
		cpu->setReg_p_c_bit(0);

	// Establece el bit ZERO del registro P
	cpu->setZeroBit(rst);

	// Establece el bit OVERFLOW del registro P
	if (((ac ^ op) & 0x80) && ((ac ^ rst) & 0x80))
		cpu->setReg_p_v_bit(1);
	else
		cpu->setReg_p_v_bit(0);

	// Establece el bit SIGN del registro P
	cpu->setSignBit(rst);

	cpu->setRegA(rst);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


// SBC_inmediate
SBC_inmediate::SBC_inmediate(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_inmediate::~SBC_inmediate() {
}

int SBC_inmediate::execute() {
	return SBC::execute(operand);
}


// SBC_zero
SBC_zero::SBC_zero(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_zero::~SBC_zero() {
}

int SBC_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_zerox
SBC_zerox::SBC_zerox(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_zerox::~SBC_zerox() {
}

int SBC_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_abs
SBC_abs::SBC_abs(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_abs::~SBC_abs() {
}

int SBC_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_absx
SBC_absx::SBC_absx(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_absx::~SBC_absx() {
}

int SBC_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_absy
SBC_absy::SBC_absy(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_absy::~SBC_absy() {
}

int SBC_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_preindexi
SBC_preindexi::SBC_preindexi(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_preindexi::~SBC_preindexi() {
}

int SBC_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


// SBC_postindexi
SBC_postindexi::SBC_postindexi(int operand, CPU* cpu) : SBC(operand, cpu) {}

SBC_postindexi::~SBC_postindexi() {
}

int SBC_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return SBC::execute(op);
}


/******************************************************************************
 * SEC Set carry flag
 *****************************************************************************/

SEC::SEC(CPU* cpu) : Instruction(0, cpu) {}

SEC::~SEC() {
}

int SEC::execute() {
	cpu->setReg_p_c_bit(1);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * SED Set decimal mode
 *****************************************************************************/

SED::SED(CPU* cpu) : Instruction(0, cpu) {}

SED::~SED() {
}

int SED::execute() {
	cpu->setReg_p_d_bit(1);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * SEI Set interrupt disable status
 *****************************************************************************/

SEI::SEI(CPU* cpu) : Instruction(0, cpu) {}

SEI::~SEI() {
}

int SEI::execute() {
	cpu->setReg_p_i_bit(1);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * STA Store accumulator in memory
 *****************************************************************************/

STA::STA(int operand, CPU* cpu) : Instruction(operand, cpu) {}

STA::~STA() {
}

int STA::execute(int addr) {
	int ac = cpu->getRegA();
	cpu->getMem()->writeData(ac, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (addr != 0x4014)
		return getCycles();
	else
		return getCycles() + 512;
}


// STA_zero
STA_zero::STA_zero(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_zero::~STA_zero() {
}

int STA_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_zerox
STA_zerox::STA_zerox(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_zerox::~STA_zerox() {
}

int STA_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_abs
STA_abs::STA_abs(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_abs::~STA_abs() {
}

int STA_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_absx
STA_absx::STA_absx(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_absx::~STA_absx() {
}

int STA_absx::execute() {
	int addr = fetchIndexedAbsXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_absy
STA_absy::STA_absy(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_absy::~STA_absy() {
}

int STA_absy::execute() {
	int addr = fetchIndexedAbsYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_preindexi
STA_preindexi::STA_preindexi(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_preindexi::~STA_preindexi() {
}

int STA_preindexi::execute() {
	int addr = fetchPreindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


// STA_postindexi
STA_postindexi::STA_postindexi(int operand, CPU* cpu) : STA(operand, cpu) {}

STA_postindexi::~STA_postindexi() {
}

int STA_postindexi::execute() {
	int addr = fetchPostindexedAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STA::execute(op);
}


/******************************************************************************
 * STX Store index X in memory
 *****************************************************************************/

STX::STX(int operand, CPU* cpu) : Instruction(operand, cpu) {}

STX::~STX() {
}

int STX::execute(int addr) {
	int regX = cpu->getRegX();
	cpu->getMem()->writeData(regX, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (addr != 0x4014)
		return getCycles();
	else
		return getCycles() + 512;
}


// STX_zero
STX_zero::STX_zero(int operand, CPU* cpu) : STX(operand, cpu) {}

STX_zero::~STX_zero() {
}

int STX_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STX::execute(op);
}


// STX_zerox
STX_zeroy::STX_zeroy(int operand, CPU* cpu) : STX(operand, cpu) {}

STX_zeroy::~STX_zeroy() {
}

int STX_zeroy::execute() {
	int addr = fetchIndexedZeroYAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STX::execute(op);
}


// STX_abs
STX_abs::STX_abs(int operand, CPU* cpu) : STX(operand, cpu) {}

STX_abs::~STX_abs() {
}

int STX_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STX::execute(op);
}


/******************************************************************************
 * STY Store index Y in memory
 *****************************************************************************/

STY::STY(int operand, CPU* cpu) : Instruction(operand, cpu) {}

STY::~STY() {
}

int STY::execute(int addr) {
	int regY = cpu->getRegY();
	cpu->getMem()->writeData(regY, addr);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	if (addr != 0x4014)
		return getCycles();
	else
		return getCycles() + 512;
}


// STY_zero
STY_zero::STY_zero(int operand, CPU* cpu) : STY(operand, cpu) {}

STY_zero::~STY_zero() {
}

int STY_zero::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STY::execute(op);
}


// STY_zerox
STY_zerox::STY_zerox(int operand, CPU* cpu) : STY(operand, cpu) {}

STY_zerox::~STY_zerox() {
}

int STY_zerox::execute() {
	int addr = fetchIndexedZeroXAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STY::execute(op);
}


// STY_abs
STY_abs::STY_abs(int operand, CPU* cpu) : STY(operand, cpu) {}

STY_abs::~STY_abs() {
}

int STY_abs::execute() {
	int addr = fetchAbsoluteAddrmode();
	int op = cpu->getMem()->readData(addr);
	return STY::execute(op);
}


/******************************************************************************
 * TAX Transfer accumulator to index X
 *****************************************************************************/

TAX::TAX(CPU* cpu) : Instruction(0, cpu) {}

TAX::~TAX() {
}

int TAX::execute() {
    int ac = cpu->getRegA();

	cpu->setZeroBit(ac);
	cpu->setSignBit(ac);

	cpu->setRegX(ac);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * TAY Transfer accumulator to index Y
 *****************************************************************************/

TAY::TAY(CPU* cpu) : Instruction(0, cpu) {}

TAY::~TAY() {
}

int TAY::execute() {
    int ac = cpu->getRegA();

	cpu->setZeroBit(ac);
	cpu->setSignBit(ac);

	cpu->setRegY(ac);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * TSX Transfer stack pointer to index X
 *****************************************************************************/

TSX::TSX(CPU* cpu) : Instruction(0, cpu) {}

TSX::~TSX() {
}

int TSX::execute() {
    int sp = cpu->getRegSp();

	cpu->setZeroBit(sp);
	cpu->setSignBit(sp);

	cpu->setRegX(sp);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * TXA Transfer index X to accumulator
 *****************************************************************************/

TXA::TXA(CPU* cpu) : Instruction(0, cpu) {}

TXA::~TXA() {
}

int TXA::execute() {
    int regX = cpu->getRegX();

	cpu->setZeroBit(regX);
	cpu->setSignBit(regX);

	cpu->setRegA(regX);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * TXS Transfer index X to stack pointer
 *****************************************************************************/

TXS::TXS(CPU* cpu) : Instruction(0, cpu) {}

TXS::~TXS() {
}

int TXS::execute() {
    int regX = cpu->getRegX();

	cpu->setRegSp(regX);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/******************************************************************************
 * TYA Transfer index Y to accumulator
 *****************************************************************************/

TYA::TYA(CPU* cpu) : Instruction(0, cpu) {}

TYA::~TYA() {
}

int TYA::execute() {
    int regY = cpu->getRegX();

	cpu->setZeroBit(regY);
	cpu->setSignBit(regY);

	cpu->setRegA(regY);

	// Incrementa el registro contador (PC) de la CPU
	cpu->incrPc(getBytes());

	return getCycles();
}


/*****************************************************************************/

// INSTRUCTIONS POOL

InstructionsPool::InstructionsPool(CPU* cpu) {
	for (int i = 0; i++; i < 0x100) {
		pool[i] = NULL;
	}

	// ADC
	pool[0x69] = new ADC_inmediate(0, cpu);
	pool[0x65] = new ADC_zero(0, cpu);
	pool[0x75] = new ADC_zerox(0, cpu);
	pool[0x6D] = new ADC_abs(0, cpu);
	pool[0x7D] = new ADC_absx(0, cpu);
	pool[0x79] = new ADC_absy(0, cpu);
	pool[0x61] = new ADC_preindexi(0, cpu);
	pool[0x71] = new ADC_postindexi(0, cpu);
	// AND
	pool[0x29] = new AND_inmediate(0, cpu);
	pool[0x25] = new AND_zero(0, cpu);
	pool[0x35] = new AND_zerox(0, cpu);
	pool[0x2D] = new AND_abs(0, cpu);
	pool[0x3D] = new AND_absx(0, cpu);
	pool[0x39] = new AND_absy(0, cpu);
	pool[0x21] = new AND_preindexi(0, cpu);
	pool[0x31] = new AND_postindexi(0, cpu);
	// ASL
	pool[0x0A] = new ASL_accumulator(0, cpu);
	pool[0x06] = new ASL_zero(0, cpu);
	pool[0x16] = new ASL_zerox(0, cpu);
	pool[0x0E] = new ASL_abs(0, cpu);
	pool[0x1E] = new ASL_absx(0, cpu);
	// BCC
	pool[0x90] = new BCC(0, cpu);
	// BCS
	pool[0xB0] = new BCS(0, cpu);
	// BEQ
	pool[0xF0] = new BEQ(0, cpu);
	// BIT
	pool[0x24] = new BIT_zero(0, cpu);
	pool[0x2C] = new BIT_abs(0, cpu);
	// BMI
	pool[0x30] = new BMI(0, cpu);
	// BNE
	pool[0xD0] = new BNE(0, cpu);
	// BPL
	pool[0x10] = new BPL(0, cpu);
	// BRK
	pool[0x00] = new BRK(cpu);
	// BVC
	pool[0x50] = new BVC(0, cpu);
	// BVS
	pool[0x70] = new BVS(0, cpu);
	// CLC
	pool[0x18] = new CLC(cpu);
	// CLD
	pool[0xD8] = new CLD(cpu);
	// CLI
	pool[0x58] = new CLI(cpu);
	// CLV
	pool[0xB8] = new CLV(cpu);
	// CMP
	pool[0xC9] = new CMP_inmediate(0, cpu);
	pool[0xC5] = new CMP_zero(0, cpu);
	pool[0xD5] = new CMP_zerox(0, cpu);
	pool[0xCD] = new CMP_abs(0, cpu);
	pool[0xDD] = new CMP_absx(0, cpu);
	pool[0xD9] = new CMP_absy(0, cpu);
	pool[0xC1] = new CMP_preindexi(0, cpu);
	pool[0xD1] = new CMP_postindexi(0, cpu);
	// CPX
	pool[0xE0] = new CPX_inmediate(0, cpu);
	pool[0xE4] = new CPX_zero(0, cpu);
	pool[0xEC] = new CPX_abs(0, cpu);
	// CPY
	pool[0xC0] = new CPY_inmediate(0, cpu);
	pool[0xC4] = new CPY_zero(0, cpu);
	pool[0xCC] = new CPY_abs(0, cpu);
	// DEC
	pool[0xC6] = new DEC_zero(0, cpu);
	pool[0xD6] = new DEC_zerox(0, cpu);
	pool[0xCE] = new DEC_abs(0, cpu);
	pool[0xDE] = new DEC_absx(0, cpu);
	// DEX
	pool[0xCA] = new DEX(cpu);
	// DEY
	pool[0x88] = new DEY(cpu);
	// EOR
	pool[0x49] = new EOR_inmediate(0, cpu);
	pool[0x45] = new EOR_zero(0, cpu);
	pool[0x55] = new EOR_zerox(0, cpu);
	pool[0x4D] = new EOR_abs(0, cpu);
	pool[0x5D] = new EOR_absx(0, cpu);
	pool[0x59] = new EOR_absy(0, cpu);
	pool[0x41] = new EOR_preindexi(0, cpu);
	pool[0x51] = new EOR_postindexi(0, cpu);
	// INC
	pool[0xE6] = new INC_zero(0, cpu);
	pool[0xF6] = new INC_zerox(0, cpu);
	pool[0xEE] = new INC_abs(0, cpu);
	pool[0xFE] = new INC_absx(0, cpu);
	// INX
	pool[0xE8] = new INX(cpu);
	// INY
	pool[0xC8] = new INY(cpu);
	// JMP
	pool[0x4C] = new JMP_abs(0, cpu);
	pool[0x6C] = new JMP_indirect(0, cpu);
	// JSR
	pool[0x20] = new JSR(0, cpu);
	// LDA
	pool[0xA9] = new LDA_inmediate(0, cpu);
	pool[0xA5] = new LDA_zero(0, cpu);
	pool[0xB5] = new LDA_zerox(0, cpu);
	pool[0xAD] = new LDA_abs(0, cpu);
	pool[0xBD] = new LDA_absx(0, cpu);
	pool[0xB9] = new LDA_absy(0, cpu);
	pool[0xA1] = new LDA_preindexi(0, cpu);
	pool[0xB1] = new LDA_postindexi(0, cpu);
	// LDX
	pool[0xA2] = new LDX_inmediate(0, cpu);
	pool[0xA6] = new LDX_zero(0, cpu);
	pool[0xB6] = new LDX_zeroy(0, cpu);
	pool[0xAE] = new LDX_abs(0, cpu);
	pool[0xBE] = new LDX_absy(0, cpu);
	// LDY
	pool[0xA0] = new LDY_inmediate(0, cpu);
	pool[0xA4] = new LDY_zero(0, cpu);
	pool[0xB4] = new LDY_zerox(0, cpu);
	pool[0xAC] = new LDY_abs(0, cpu);
	pool[0xBC] = new LDY_absx(0, cpu);
	// LSR
	pool[0x4A] = new LSR_accumulator(0, cpu);
	pool[0x46] = new LSR_zero(0, cpu);
	pool[0x56] = new LSR_zerox(0, cpu);
	pool[0x4E] = new LSR_abs(0, cpu);
	pool[0x5E] = new LSR_absx(0, cpu);
	// NOP
	pool[0xEA] = new NOP(cpu);
	// ORA
	pool[0x09] = new ORA_inmediate(0, cpu);
	pool[0x05] = new ORA_zero(0, cpu);
	pool[0x15] = new ORA_zerox(0, cpu);
	pool[0x0D] = new ORA_abs(0, cpu);
	pool[0x1D] = new ORA_absx(0, cpu);
	pool[0x19] = new ORA_absy(0, cpu);
	pool[0x01] = new ORA_preindexi(0, cpu);
	pool[0x11] = new ORA_postindexi(0, cpu);
	// PHA
	pool[0x48] = new PHA(cpu);
	// PHP
	pool[0x08] = new PHP(cpu);
	// PLA
	pool[0x68] = new PLA(cpu);
	// PLP
	pool[0x28] = new PLP(cpu);
	// ROL
	pool[0x2A] = new ROL_accumulator(0, cpu);
	pool[0x26] = new ROL_zero(0, cpu);
	pool[0x36] = new ROL_zerox(0, cpu);
	pool[0x2E] = new ROL_abs(0, cpu);
	pool[0x3E] = new ROL_absx(0, cpu);
	// ROR
	pool[0x6A] = new ROR_accumulator(0, cpu);
	pool[0x66] = new ROR_zero(0, cpu);
	pool[0x76] = new ROR_zerox(0, cpu);
	pool[0x6E] = new ROR_abs(0, cpu);
	pool[0x7E] = new ROR_absx(0, cpu);
	// RTI
	pool[0x40] = new RTI(cpu);
	// RTS
	pool[0x60] = new RTS(cpu);
	// SBC
	pool[0xE9] = new SBC_inmediate(0, cpu);
	pool[0xE5] = new SBC_zero(0, cpu);
	pool[0xF5] = new SBC_zerox(0, cpu);
	pool[0xED] = new SBC_abs(0, cpu);
	pool[0xFD] = new SBC_absx(0, cpu);
	pool[0xF9] = new SBC_absy(0, cpu);
	pool[0xE1] = new SBC_preindexi(0, cpu);
	pool[0xF1] = new SBC_postindexi(0, cpu);
	// SEC
	pool[0x38] = new SEC(cpu);
	// SED
	pool[0xF8] = new SED(cpu);
	// SEI
	pool[0x78] = new SEI(cpu);
	// STA
	pool[0x85] = new STA_zero(0, cpu);
	pool[0x95] = new STA_zerox(0, cpu);
	pool[0x8D] = new STA_abs(0, cpu);
	pool[0x9D] = new STA_absx(0, cpu);
	pool[0x99] = new STA_absy(0, cpu);
	pool[0x81] = new STA_preindexi(0, cpu);
	pool[0x91] = new STA_postindexi(0, cpu);
	// STX
	pool[0x86] = new STX_zero(0, cpu);
	pool[0x96] = new STX_zeroy(0, cpu);
	pool[0x8E] = new STX_abs(0, cpu);
	// STY
	pool[0x84] = new STY_zero(0, cpu);
	pool[0x94] = new STY_zerox(0, cpu);
	pool[0x8C] = new STY_abs(0, cpu);
	// TAX
	pool[0xAA] = new TAX(cpu);
	// TAY
	pool[0xA8] = new TAY(cpu);
	// TSX
	pool[0xBA] = new TSX(cpu);
	// TXA
	pool[0x8A] = new TXA(cpu);
	// TXS
	pool[0x9A] = new TXS(cpu);
	// TYA
	pool[0x98] = new TYA(cpu);
}


InstructionsPool::~InstructionsPool() {
	for (int i = 0; i++; i < 0x100) {
		if (pool[i] != NULL)
			delete pool[i];
	}
}

int ADC_inmediate::getOpcode() {
}

int ADC_inmediate::getBytes() {
}

int ADC_inmediate::getCycles() {
}

int ADC_zero::getOpcode() {
}

int ADC_zero::getBytes() {
}

int ADC_zero::getCycles() {
}

int ADC_zerox::getOpcode() {
}

int ADC_zerox::getBytes() {
}

int ADC_zerox::getCycles() {
}

int ADC_abs::getOpcode() {
}

int ADC_abs::getBytes() {
}

int ADC_abs::getCycles() {
}

int ADC_absx::getOpcode() {
}

int ADC_absx::getBytes() {
}

int ADC_absx::getCycles() {
}

int ADC_absy::getOpcode() {
}

int ADC_absy::getBytes() {
}

int ADC_absy::getCycles() {
}

int ADC_preindexi::getOpcode() {
}

int ADC_preindexi::getBytes() {
}

int ADC_preindexi::getCycles() {
}

int ADC_postindexi::getOpcode() {
}

int ADC_postindexi::getBytes() {
}

int ADC_postindexi::getCycles() {
}

int AND_inmediate::getOpcode() {
}

int AND_inmediate::getBytes() {
}

int AND_inmediate::getCycles() {
}

int AND_zero::getOpcode() {
}

int AND_zero::getBytes() {
}

int AND_zero::getCycles() {
}

int AND_zerox::getOpcode() {
}

int AND_zerox::getBytes() {
}

int AND_zerox::getCycles() {
}

int AND_abs::getOpcode() {
}

int AND_abs::getBytes() {
}

int AND_abs::getCycles() {
}

int AND_absx::getOpcode() {
}

int AND_absx::getBytes() {
}

int AND_absx::getCycles() {
}

int AND_absy::getOpcode() {
}

int AND_absy::getBytes() {
}

int AND_absy::getCycles() {
}

int AND_preindexi::getOpcode() {
}

int AND_preindexi::getBytes() {
}

int AND_preindexi::getCycles() {
}

int AND_postindexi::getOpcode() {
}

int AND_postindexi::getBytes() {
}

int AND_postindexi::getCycles() {
}

int ASL_accumulator::getOpcode() {
}

int ASL_accumulator::getBytes() {
}

int ASL_accumulator::getCycles() {
}

int ASL_zero::getOpcode() {
}

int ASL_zero::getBytes() {
}

int ASL_zero::getCycles() {
}

int ASL_zerox::getOpcode() {
}

int ASL_zerox::getBytes() {
}

int ASL_zerox::getCycles() {
}

int ASL_abs::getOpcode() {
}

int ASL_abs::getBytes() {
}

int ASL_abs::getCycles() {
}

int ASL_absx::getOpcode() {
}

int ASL_absx::getBytes() {
}

int ASL_absx::getCycles() {
}

int BCC::getOpcode() {
}

int BCC::getBytes() {
}

int BCC::getCycles() {
}

int BCS::getOpcode() {
}

int BCS::getBytes() {
}

int BCS::getCycles() {
}

int BEQ::getOpcode() {
}

int BEQ::getBytes() {
}

int BEQ::getCycles() {
}

int BIT_zero::getOpcode() {
}

int BIT_zero::getBytes() {
}

int BIT_zero::getCycles() {
}

int BIT_abs::getOpcode() {
}

int BIT_abs::getBytes() {
}

int BIT_abs::getCycles() {
}

int BMI::getOpcode() {
}

int BMI::getBytes() {
}

int BMI::getCycles() {
}

int BNE::getOpcode() {
}

int BNE::getBytes() {
}

int BNE::getCycles() {
}

int BPL::getOpcode() {
}

int BPL::getBytes() {
}

int BPL::getCycles() {
}

int BRK::getOpcode() {
}

int BRK::getBytes() {
}

int BRK::getCycles() {
}

int BVC::getOpcode() {
}

int BVC::getBytes() {
}

int BVC::getCycles() {
}

int BVS::getOpcode() {
}

int BVS::getBytes() {
}

int BVS::getCycles() {
}

int CLC::getOpcode() {
}

int CLC::getBytes() {
}

int CLC::getCycles() {
}

int CLD::getOpcode() {
}

int CLD::getBytes() {
}

int CLD::getCycles() {
}

int CLI::getOpcode() {
}

int CLI::getBytes() {
}

int CLI::getCycles() {
}

int CLV::getOpcode() {
}

int CLV::getBytes() {
}

int CLV::getCycles() {
}

int CMP_inmediate::getOpcode() {
}

int CMP_inmediate::getBytes() {
}

int CMP_inmediate::getCycles() {
}

int CMP_zero::getOpcode() {
}

int CMP_zero::getBytes() {
}

int CMP_zero::getCycles() {
}

int CMP_zerox::getOpcode() {
}

int CMP_zerox::getBytes() {
}

int CMP_zerox::getCycles() {
}

int CMP_abs::getOpcode() {
}

int CMP_abs::getBytes() {
}

int CMP_abs::getCycles() {
}

int CMP_absx::getOpcode() {
}

int CMP_absx::getBytes() {
}

int CMP_absx::getCycles() {
}

int CMP_absy::getOpcode() {
}

int CMP_absy::getBytes() {
}

int CMP_absy::getCycles() {
}

int CMP_preindexi::getOpcode() {
}

int CMP_preindexi::getBytes() {
}

int CMP_preindexi::getCycles() {
}

int CMP_postindexi::getOpcode() {
}

int CMP_postindexi::getBytes() {
}

int CMP_postindexi::getCycles() {
}

int CPX_inmediate::getOpcode() {
}

int CPX_inmediate::getBytes() {
}

int CPX_inmediate::getCycles() {
}

int CPX_zero::getOpcode() {
}

int CPX_zero::getBytes() {
}

int CPX_zero::getCycles() {
}

int CPX_abs::getOpcode() {
}

int CPX_abs::getBytes() {
}

int CPX_abs::getCycles() {
}

int CPY_inmediate::getOpcode() {
}

int CPY_inmediate::getBytes() {
}

int CPY_inmediate::getCycles() {
}

int CPY_zero::getOpcode() {
}

int CPY_zero::getBytes() {
}

int CPY_zero::getCycles() {
}

int CPY_abs::getOpcode() {
}

int CPY_abs::getBytes() {
}

int CPY_abs::getCycles() {
}

int DEC_zero::getOpcode() {
}

int DEC_zero::getBytes() {
}

int DEC_zero::getCycles() {
}

int DEC_zerox::getOpcode() {
}

int DEC_zerox::getBytes() {
}

int DEC_zerox::getCycles() {
}

int DEC_abs::getOpcode() {
}

int DEC_abs::getBytes() {
}

int DEC_abs::getCycles() {
}

int DEC_absx::getOpcode() {
}

int DEC_absx::getBytes() {
}

int DEC_absx::getCycles() {
}

int DEX::getOpcode() {
}

int DEX::getBytes() {
}

int DEX::getCycles() {
}

int DEY::getOpcode() {
}

int DEY::getBytes() {
}

int DEY::getCycles() {
}

int EOR_inmediate::getOpcode() {
}

int EOR_inmediate::getBytes() {
}

int EOR_inmediate::getCycles() {
}

int EOR_zero::getOpcode() {
}

int EOR_zero::getBytes() {
}

int EOR_zero::getCycles() {
}

int EOR_zerox::getOpcode() {
}

int EOR_zerox::getBytes() {
}

int EOR_zerox::getCycles() {
}

int EOR_abs::getOpcode() {
}

int EOR_abs::getBytes() {
}

int EOR_abs::getCycles() {
}

int EOR_absx::getOpcode() {
}

int EOR_absx::getBytes() {
}

int EOR_absx::getCycles() {
}

int EOR_absy::getOpcode() {
}

int EOR_absy::getBytes() {
}

int EOR_absy::getCycles() {
}

int EOR_preindexi::getOpcode() {
}

int EOR_preindexi::getBytes() {
}

int EOR_preindexi::getCycles() {
}

int EOR_postindexi::getOpcode() {
}

int EOR_postindexi::getBytes() {
}

int EOR_postindexi::getCycles() {
}

int INC_zero::getOpcode() {
}

int INC_zero::getBytes() {
}

int INC_zero::getCycles() {
}

int INC_zerox::getOpcode() {
}

int INC_zerox::getBytes() {
}

int INC_zerox::getCycles() {
}

int INC_abs::getOpcode() {
}

int INC_abs::getBytes() {
}

int INC_abs::getCycles() {
}

int INC_absx::getOpcode() {
}

int INC_absx::getBytes() {
}

int INC_absx::getCycles() {
}

int INX::getOpcode() {
}

int INX::getBytes() {
}

int INX::getCycles() {
}

int INY::getOpcode() {
}

int INY::getBytes() {
}

int INY::getCycles() {
}

int JMP_abs::getOpcode() {
}

int JMP_abs::getBytes() {
}

int JMP_abs::getCycles() {
}

int JMP_indirect::getOpcode() {
}

int JMP_indirect::getBytes() {
}

int JMP_indirect::getCycles() {
}

int JSR::getOpcode() {
}

int JSR::getBytes() {
}

int JSR::getCycles() {
}

int LDA_inmediate::getOpcode() {
}

int LDA_inmediate::getBytes() {
}

int LDA_inmediate::getCycles() {
}

int LDA_zero::getOpcode() {
}

int LDA_zero::getBytes() {
}

int LDA_zero::getCycles() {
}

int LDA_zerox::getOpcode() {
}

int LDA_zerox::getBytes() {
}

int LDA_zerox::getCycles() {
}

int LDA_abs::getOpcode() {
}

int LDA_abs::getBytes() {
}

int LDA_abs::getCycles() {
}

int LDA_absx::getOpcode() {
}

int LDA_absx::getBytes() {
}

int LDA_absx::getCycles() {
}

int LDA_absy::getOpcode() {
}

int LDA_absy::getBytes() {
}

int LDA_absy::getCycles() {
}

int LDA_preindexi::getOpcode() {
}

int LDA_preindexi::getBytes() {
}

int LDA_preindexi::getCycles() {
}

int LDA_postindexi::getOpcode() {
}

int LDA_postindexi::getBytes() {
}

int LDA_postindexi::getCycles() {
}

int LDX_inmediate::getOpcode() {
}

int LDX_inmediate::getBytes() {
}

int LDX_inmediate::getCycles() {
}

int LDX_zero::getOpcode() {
}

int LDX_zero::getBytes() {
}

int LDX_zero::getCycles() {
}

int LDX_zeroy::getOpcode() {
}

int LDX_zeroy::getBytes() {
}

int LDX_zeroy::getCycles() {
}

int LDX_abs::getOpcode() {
}

int LDX_abs::getBytes() {
}

int LDX_abs::getCycles() {
}

int LDX_absy::getOpcode() {
}

int LDX_absy::getBytes() {
}

int LDX_absy::getCycles() {
}

int LDY_inmediate::getOpcode() {
}

int LDY_inmediate::getBytes() {
}

int LDY_inmediate::getCycles() {
}

int LDY_zero::getOpcode() {
}

int LDY_zero::getBytes() {
}

int LDY_zero::getCycles() {
}

int LDY_zerox::getOpcode() {
}

int LDY_zerox::getBytes() {
}

int LDY_zerox::getCycles() {
}

int LDY_abs::getOpcode() {
}

int LDY_abs::getBytes() {
}

int LDY_abs::getCycles() {
}

int LDY_absx::getOpcode() {
}

int LDY_absx::getBytes() {
}

int LDY_absx::getCycles() {
}

int LSR_accumulator::getOpcode() {
}

int LSR_accumulator::getBytes() {
}

int LSR_accumulator::getCycles() {
}

int LSR_zero::getOpcode() {
}

int LSR_zero::getBytes() {
}

int LSR_zero::getCycles() {
}

int LSR_zerox::getOpcode() {
}

int LSR_zerox::getBytes() {
}

int LSR_zerox::getCycles() {
}

int LSR_abs::getOpcode() {
}

int LSR_abs::getBytes() {
}

int LSR_abs::getCycles() {
}

int LSR_absx::getOpcode() {
}

int LSR_absx::getBytes() {
}

int LSR_absx::getCycles() {
}

int NOP::getOpcode() {
}

int NOP::getBytes() {
}

int NOP::getCycles() {
}

int ORA_inmediate::getOpcode() {
}

int ORA_inmediate::getBytes() {
}

int ORA_inmediate::getCycles() {
}

int ORA_zero::getOpcode() {
}

int ORA_zero::getBytes() {
}

int ORA_zero::getCycles() {
}

int ORA_zerox::getOpcode() {
}

int ORA_zerox::getBytes() {
}

int ORA_zerox::getCycles() {
}

int ORA_abs::getOpcode() {
}

int ORA_abs::getBytes() {
}

int ORA_abs::getCycles() {
}

int ORA_absx::getOpcode() {
}

int ORA_absx::getBytes() {
}

int ORA_absx::getCycles() {
}

int ORA_absy::getOpcode() {
}

int ORA_absy::getBytes() {
}

int ORA_absy::getCycles() {
}

int ORA_preindexi::getOpcode() {
}

int ORA_preindexi::getBytes() {
}

int ORA_preindexi::getCycles() {
}

int ORA_postindexi::getOpcode() {
}

int ORA_postindexi::getBytes() {
}

int ORA_postindexi::getCycles() {
}

int PHA::getOpcode() {
}

int PHA::getBytes() {
}

int PHA::getCycles() {
}

int PHP::getOpcode() {
}

int PHP::getBytes() {
}

int PHP::getCycles() {
}

int PLA::getOpcode() {
}

int PLA::getBytes() {
}

int PLA::getCycles() {
}

int PLP::getOpcode() {
}

int PLP::getBytes() {
}

int PLP::getCycles() {
}

int ROL_accumulator::getOpcode() {
}

int ROL_accumulator::getBytes() {
}

int ROL_accumulator::getCycles() {
}

int ROL_zero::getOpcode() {
}

int ROL_zero::getBytes() {
}

int ROL_zero::getCycles() {
}

int ROL_zerox::getOpcode() {
}

int ROL_zerox::getBytes() {
}

int ROL_zerox::getCycles() {
}

int ROL_abs::getOpcode() {
}

int ROL_abs::getBytes() {
}

int ROL_abs::getCycles() {
}

int ROL_absx::getOpcode() {
}

int ROL_absx::getBytes() {
}

int ROL_absx::getCycles() {
}

int ROR_accumulator::getOpcode() {
}

int ROR_accumulator::getBytes() {
}

int ROR_accumulator::getCycles() {
}

int ROR_zero::getOpcode() {
}

int ROR_zero::getBytes() {
}

int ROR_zero::getCycles() {
}

int ROR_zerox::getOpcode() {
}

int ROR_zerox::getBytes() {
}

int ROR_zerox::getCycles() {
}

int ROR_abs::getOpcode() {
}

int ROR_abs::getBytes() {
}

int ROR_abs::getCycles() {
}

int ROR_absx::getOpcode() {
}

int ROR_absx::getBytes() {
}

int ROR_absx::getCycles() {
}

int RTI::getOpcode() {
}

int RTI::getBytes() {
}

int RTI::getCycles() {
}

int RTS::getOpcode() {
}

int RTS::getBytes() {
}

int RTS::getCycles() {
}

int SBC_inmediate::getOpcode() {
}

int SBC_inmediate::getBytes() {
}

int SBC_inmediate::getCycles() {
}

int SBC_zero::getOpcode() {
}

int SBC_zero::getBytes() {
}

int SBC_zero::getCycles() {
}

int SBC_zerox::getOpcode() {
}

int SBC_zerox::getBytes() {
}

int SBC_zerox::getCycles() {
}

int SBC_abs::getOpcode() {
}

int SBC_abs::getBytes() {
}

int SBC_abs::getCycles() {
}

int SBC_absx::getOpcode() {
}

int SBC_absx::getBytes() {
}

int SBC_absx::getCycles() {
}

int SBC_absy::getOpcode() {
}

int SBC_absy::getBytes() {
}

int SBC_absy::getCycles() {
}

int SBC_preindexi::getOpcode() {
}

int SBC_preindexi::getBytes() {
}

int SBC_preindexi::getCycles() {
}

int SBC_postindexi::getOpcode() {
}

int SBC_postindexi::getBytes() {
}

int SBC_postindexi::getCycles() {
}

int SEC::getOpcode() {
}

int SEC::getBytes() {
}

int SEC::getCycles() {
}

int SED::getOpcode() {
}

int SED::getBytes() {
}

int SED::getCycles() {
}

int SEI::getOpcode() {
}

int SEI::getBytes() {
}

int SEI::getCycles() {
}

int STA_zero::getOpcode() {
}

int STA_zero::getBytes() {
}

int STA_zero::getCycles() {
}

int STA_zerox::getOpcode() {
}

int STA_zerox::getBytes() {
}

int STA_zerox::getCycles() {
}

int STA_abs::getOpcode() {
}

int STA_abs::getBytes() {
}

int STA_abs::getCycles() {
}

int STA_absx::getOpcode() {
}

int STA_absx::getBytes() {
}

int STA_absx::getCycles() {
}

int STA_absy::getOpcode() {
}

int STA_absy::getBytes() {
}

int STA_absy::getCycles() {
}

int STA_preindexi::getOpcode() {
}

int STA_preindexi::getBytes() {
}

int STA_preindexi::getCycles() {
}

int STA_postindexi::getOpcode() {
}

int STA_postindexi::getBytes() {
}

int STA_postindexi::getCycles() {
}

int STX_zero::getOpcode() {
}

int STX_zero::getBytes() {
}

int STX_zero::getCycles() {
}

int STX_zeroy::getOpcode() {
}

int STX_zeroy::getBytes() {
}

int STX_zeroy::getCycles() {
}

int STX_abs::getOpcode() {
}

int STX_abs::getBytes() {
}

int STX_abs::getCycles() {
}

int STY_zero::getOpcode() {
}

int STY_zero::getBytes() {
}

int STY_zero::getCycles() {
}

int STY_zerox::getOpcode() {
}

int STY_zerox::getBytes() {
}

int STY_zerox::getCycles() {
}

int STY_abs::getOpcode() {
}

int STY_abs::getBytes() {
}

int STY_abs::getCycles() {
}

int TAX::getOpcode() {
}

int TAX::getBytes() {
}

int TAX::getCycles() {
}

int TAY::getOpcode() {
}

int TAY::getBytes() {
}

int TAY::getCycles() {
}

int TSX::getOpcode() {
}

int TSX::getBytes() {
}

int TSX::getCycles() {
}

int TXA::getOpcode() {
}

int TXA::getBytes() {
}

int TXA::getCycles() {
}

int TXS::getOpcode() {
}

int TXS::getBytes() {
}

int TXS::getCycles() {
}

int TYA::getOpcode() {
}

int TYA::getBytes() {
}

int TYA::getCycles() {
}
