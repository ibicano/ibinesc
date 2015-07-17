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
	return ADC_inmediate::OPCODE;
}

int ADC_inmediate::getBytes() {
	return ADC_inmediate::BYTES;
}

int ADC_inmediate::getCycles() {
	return ADC_inmediate::CYCLES;
}

int ADC_zero::getOpcode() {
	return ADC_zero::OPCODE;
}

int ADC_zero::getBytes() {
	return ADC_zero::BYTES;
}

int ADC_zero::getCycles() {
	return ADC_zero::CYCLES;
}

int ADC_zerox::getOpcode() {
	return ADC_zerox::OPCODE;
}

int ADC_zerox::getBytes() {
	return ADC_zerox::BYTES;
}

int ADC_zerox::getCycles() {
	return ADC_zerox::CYCLES;
}

int ADC_abs::getOpcode() {
	return ADC_abs::OPCODE;
}

int ADC_abs::getBytes() {
	return ADC_abs::BYTES;
}

int ADC_abs::getCycles() {
	return ADC_abs::CYCLES;
}

int ADC_absx::getOpcode() {
	return ADC_absx::OPCODE;
}

int ADC_absx::getBytes() {
	return ADC_absx::BYTES;
}

int ADC_absx::getCycles() {
	return ADC_absx::CYCLES;
}

int ADC_absy::getOpcode() {
	return ADC_absy::OPCODE;
}

int ADC_absy::getBytes() {
	return ADC_absy::BYTES;
}

int ADC_absy::getCycles() {
	return ADC_absy::CYCLES;
}

int ADC_preindexi::getOpcode() {
	return ADC_preindexi::OPCODE;
}

int ADC_preindexi::getBytes() {
	return ADC_preindexi::BYTES;
}

int ADC_preindexi::getCycles() {
	return ADC_preindexi::CYCLES;
}

int ADC_postindexi::getOpcode() {
	return ADC_postindexi::OPCODE;
}

int ADC_postindexi::getBytes() {
	return ADC_postindexi::BYTES;
}

int ADC_postindexi::getCycles() {
	return ADC_postindexi::CYCLES;
}

int AND_inmediate::getOpcode() {
	return AND_inmediate::OPCODE;
}

int AND_inmediate::getBytes() {
	return AND_inmediate::BYTES;
}

int AND_inmediate::getCycles() {
	return AND_inmediate::CYCLES;
}

int AND_zero::getOpcode() {
	return AND_zero::OPCODE;
}

int AND_zero::getBytes() {
	return AND_zero::BYTES;
}

int AND_zero::getCycles() {
	return AND_zero::CYCLES;
}

int AND_zerox::getOpcode() {
	return AND_zerox::OPCODE;
}

int AND_zerox::getBytes() {
	return AND_zerox::BYTES;
}

int AND_zerox::getCycles() {
	return AND_zerox::CYCLES;
}

int AND_abs::getOpcode() {
	return AND_abs::OPCODE;
}

int AND_abs::getBytes() {
	return AND_abs::BYTES;
}

int AND_abs::getCycles() {
	return AND_abs::CYCLES;
}

int AND_absx::getOpcode() {
	return AND_absx::OPCODE;
}

int AND_absx::getBytes() {
	return AND_absx::BYTES;
}

int AND_absx::getCycles() {
	return AND_absx::CYCLES;
}

int AND_absy::getOpcode() {
	return AND_absy::OPCODE;
	return AND_absy::OPCODE;
}

int AND_absy::getBytes() {
	return AND_absy::BYTES;
}

int AND_absy::getCycles() {
	return AND_absy::CYCLES;
}

int AND_preindexi::getOpcode() {
	return AND_preindexi::OPCODE;
}

int AND_preindexi::getBytes() {
	return AND_preindexi::BYTES;
}

int AND_preindexi::getCycles() {
	return AND_preindexi::CYCLES;
}

int AND_postindexi::getOpcode() {
	return AND_postindexi::OPCODE;
}

int AND_postindexi::getBytes() {
	return AND_postindexi::BYTES;
}

int AND_postindexi::getCycles() {
	return AND_postindexi::CYCLES;
}

int ASL_accumulator::getOpcode() {
	return ASL_accumulator::OPCODE;
}

int ASL_accumulator::getBytes() {
	return ASL_accumulator::BYTES;
}

int ASL_accumulator::getCycles() {
	return ASL_accumulator::CYCLES;
}

int ASL_zero::getOpcode() {
	return ASL_zero::OPCODE;
}

int ASL_zero::getBytes() {
	return ASL_zero::BYTES;
}

int ASL_zero::getCycles() {
	return ASL_zero::CYCLES;
}

int ASL_zerox::getOpcode() {
	return ASL_zerox::OPCODE;
}

int ASL_zerox::getBytes() {
	return ASL_zerox::BYTES;
}

int ASL_zerox::getCycles() {
	return ASL_zerox::CYCLES;
}

int ASL_abs::getOpcode() {
	return ASL_abs::OPCODE;
}

int ASL_abs::getBytes() {
	return ASL_abs::BYTES;
}

int ASL_abs::getCycles() {
	return ASL_abs::CYCLES;
}

int ASL_absx::getOpcode() {
	return ASL_absx::OPCODE;
}

int ASL_absx::getBytes() {
	return ASL_absx::BYTES;
}

int ASL_absx::getCycles() {
	return ASL_absx::CYCLES;
}

int BCC::getOpcode() {
	return BCC::OPCODE;
}

int BCC::getBytes() {
	return BCC::BYTES;
}

int BCC::getCycles() {
	return BCC::CYCLES;
}

int BCS::getOpcode() {
	return BCS::OPCODE;
}

int BCS::getBytes() {
	return BCS::BYTES;
}

int BCS::getCycles() {
	return BCS::CYCLES;
}

int BEQ::getOpcode() {
	return BEQ::OPCODE;
}

int BEQ::getBytes() {
	return BEQ::BYTES;
}

int BEQ::getCycles() {
	return BEQ::CYCLES;
}

int BIT_zero::getOpcode() {
	return BIT_zero::OPCODE;
}

int BIT_zero::getBytes() {
	return BIT_zero::BYTES;
}

int BIT_zero::getCycles() {
	return BIT_zero::CYCLES;
}

int BIT_abs::getOpcode() {
	return BIT_abs::OPCODE;
}

int BIT_abs::getBytes() {
	return BIT_abs::BYTES;
}

int BIT_abs::getCycles() {
	return BIT_abs::CYCLES;
}

int BMI::getOpcode() {
	return BMI::OPCODE;
}

int BMI::getBytes() {
	return BMI::BYTES;
}

int BMI::getCycles() {
	return BMI::CYCLES;
}

int BNE::getOpcode() {
	return BNE::OPCODE;
}

int BNE::getBytes() {
	return BNE::BYTES;
}

int BNE::getCycles() {
	return BNE::CYCLES;
}

int BPL::getOpcode() {
	return BPL::OPCODE;
}

int BPL::getBytes() {
	return BPL::BYTES;
}

int BPL::getCycles() {
	return BPL::CYCLES;
}

int BRK::getOpcode() {
	return BRK::OPCODE;
}

int BRK::getBytes() {
	return BRK::BYTES;
}

int BRK::getCycles() {
	return BRK::CYCLES;
}

int BVC::getOpcode() {
	return BVC::OPCODE;
}

int BVC::getBytes() {
	return BVC::BYTES;
}

int BVC::getCycles() {
	return BVC::CYCLES;
}

int BVS::getOpcode() {
	return BVS::OPCODE;
}

int BVS::getBytes() {
	return BVS::BYTES;
}

int BVS::getCycles() {
	return BVS::CYCLES;
}

int CLC::getOpcode() {
	return CLC::OPCODE;
}

int CLC::getBytes() {
	return CLC::BYTES;
}

int CLC::getCycles() {
	return CLC::CYCLES;
}

int CLD::getOpcode() {
	return CLD::OPCODE;
}

int CLD::getBytes() {
	return CLD::BYTES;
}

int CLD::getCycles() {
	return CLD::CYCLES;
}

int CLI::getOpcode() {
	return CLI::OPCODE;
}

int CLI::getBytes() {
	return CLI::BYTES;
}

int CLI::getCycles() {
	return CLI::CYCLES;
}

int CLV::getOpcode() {
	return CLV::OPCODE;
}

int CLV::getBytes() {
	return CLV::BYTES;
}

int CLV::getCycles() {
	return CLV::CYCLES;
}

int CMP_inmediate::getOpcode() {
	return CMP_inmediate::OPCODE;
}

int CMP_inmediate::getBytes() {
	return CMP_inmediate::BYTES;
}

int CMP_inmediate::getCycles() {
	return CMP_inmediate::CYCLES;
}

int CMP_zero::getOpcode() {
	return CMP_zero::OPCODE;
}

int CMP_zero::getBytes() {
	return CMP_zero::BYTES;
}

int CMP_zero::getCycles() {
	return CMP_zero::CYCLES;
}

int CMP_zerox::getOpcode() {
	return CMP_zerox::OPCODE;
}

int CMP_zerox::getBytes() {
	return CMP_zerox::BYTES;
}

int CMP_zerox::getCycles() {
	return CMP_zerox::CYCLES;
}

int CMP_abs::getOpcode() {
	return CMP_abs::OPCODE;
}

int CMP_abs::getBytes() {
	return CMP_abs::BYTES;
}

int CMP_abs::getCycles() {
	return CMP_abs::CYCLES;
}

int CMP_absx::getOpcode() {
	return CMP_absx::OPCODE;
}

int CMP_absx::getBytes() {
	return CMP_absx::BYTES;
}

int CMP_absx::getCycles() {
	return CMP_absx::CYCLES;
}

int CMP_absy::getOpcode() {
	return CMP_absy::OPCODE;
}

int CMP_absy::getBytes() {
	return CMP_absy::BYTES;
}

int CMP_absy::getCycles() {
	return CMP_absy::CYCLES;
}

int CMP_preindexi::getOpcode() {
	return CMP_preindexi::OPCODE;
}

int CMP_preindexi::getBytes() {
	return CMP_preindexi::BYTES;
}

int CMP_preindexi::getCycles() {
	return CMP_preindexi::CYCLES;
}

int CMP_postindexi::getOpcode() {
	return CMP_postindexi::OPCODE;
}

int CMP_postindexi::getBytes() {
	return CMP_postindexi::BYTES;
}

int CMP_postindexi::getCycles() {
	return CMP_postindexi::CYCLES;
}

int CPX_inmediate::getOpcode() {
	return CPX_inmediate::OPCODE;
}

int CPX_inmediate::getBytes() {
	return CPX_inmediate::BYTES;
}

int CPX_inmediate::getCycles() {
	return CPX_inmediate::CYCLES;
}

int CPX_zero::getOpcode() {
	return CPX_zero::OPCODE;
}

int CPX_zero::getBytes() {
	return CPX_zero::BYTES;
}

int CPX_zero::getCycles() {
	return CPX_zero::CYCLES;
}

int CPX_abs::getOpcode() {
	return CPX_abs::OPCODE;
}

int CPX_abs::getBytes() {
	return CPX_abs::BYTES;
}

int CPX_abs::getCycles() {
	return CPX_abs::CYCLES;
}

int CPY_inmediate::getOpcode() {
	return CPY_inmediate::OPCODE;
}

int CPY_inmediate::getBytes() {
	return CPY_inmediate::BYTES;
}

int CPY_inmediate::getCycles() {
	return CPY_inmediate::CYCLES;
}

int CPY_zero::getOpcode() {
	return CPY_zero::OPCODE;
}

int CPY_zero::getBytes() {
	return CPY_zero::BYTES;
}

int CPY_zero::getCycles() {
	return CPY_zero::CYCLES;
}

int CPY_abs::getOpcode() {
	return CPY_abs::OPCODE;
}

int CPY_abs::getBytes() {
	return CPY_abs::BYTES;
}

int CPY_abs::getCycles() {
	return CPY_abs::CYCLES;
}

int DEC_zero::getOpcode() {
	return DEC_zero::OPCODE;
}

int DEC_zero::getBytes() {
	return DEC_zero::BYTES;
}

int DEC_zero::getCycles() {
	return DEC_zero::CYCLES;
}

int DEC_zerox::getOpcode() {
	return DEC_zerox::OPCODE;
}

int DEC_zerox::getBytes() {
	return DEC_zerox::BYTES;
}

int DEC_zerox::getCycles() {
	return DEC_zerox::CYCLES;
}

int DEC_abs::getOpcode() {
	return DEC_abs::OPCODE;
}

int DEC_abs::getBytes() {
	return DEC_abs::BYTES;
}

int DEC_abs::getCycles() {
	return DEC_abs::CYCLES;
}

int DEC_absx::getOpcode() {
	return DEC_absx::OPCODE;
}

int DEC_absx::getBytes() {
	return DEC_absx::BYTES;
}

int DEC_absx::getCycles() {
	return DEC_absx::CYCLES;
}

int DEX::getOpcode() {
	return DEX::OPCODE;
}

int DEX::getBytes() {
	return DEX::BYTES;
}

int DEX::getCycles() {
	return DEX::CYCLES;
}

int DEY::getOpcode() {
	return DEY::OPCODE;
}

int DEY::getBytes() {
	return DEY::BYTES;
}

int DEY::getCycles() {
	return DEY::CYCLES;
}

int EOR_inmediate::getOpcode() {
	return EOR_inmediate::OPCODE;
}

int EOR_inmediate::getBytes() {
	return EOR_inmediate::BYTES;
}

int EOR_inmediate::getCycles() {
	return EOR_inmediate::CYCLES;
}

int EOR_zero::getOpcode() {
	return EOR_zero::OPCODE;
}

int EOR_zero::getBytes() {
	return EOR_zero::BYTES;
}

int EOR_zero::getCycles() {
	return EOR_zero::CYCLES;
}

int EOR_zerox::getOpcode() {
	return EOR_zerox::OPCODE;
}

int EOR_zerox::getBytes() {
	return EOR_zerox::BYTES;
}

int EOR_zerox::getCycles() {
	return EOR_zerox::CYCLES;
}

int EOR_abs::getOpcode() {
	return EOR_abs::OPCODE;
}

int EOR_abs::getBytes() {
	return EOR_abs::BYTES;
}

int EOR_abs::getCycles() {
	return EOR_abs::CYCLES;
}

int EOR_absx::getOpcode() {
	return EOR_absx::OPCODE;
}

int EOR_absx::getBytes() {
	return EOR_absx::BYTES;
}

int EOR_absx::getCycles() {
	return EOR_absx::CYCLES;
}

int EOR_absy::getOpcode() {
	return EOR_absy::OPCODE;
}

int EOR_absy::getBytes() {
	return EOR_absy::BYTES;
}

int EOR_absy::getCycles() {
	return EOR_absy::CYCLES;
}

int EOR_preindexi::getOpcode() {
	return EOR_preindexi::OPCODE;
}

int EOR_preindexi::getBytes() {
	return EOR_preindexi::BYTES;
}

int EOR_preindexi::getCycles() {
	return EOR_preindexi::CYCLES;
}

int EOR_postindexi::getOpcode() {
	return EOR_postindexi::OPCODE;
}

int EOR_postindexi::getBytes() {
	return EOR_postindexi::BYTES;
}

int EOR_postindexi::getCycles() {
	return EOR_postindexi::CYCLES;
}

int INC_zero::getOpcode() {
	return INC_zero::OPCODE;
}

int INC_zero::getBytes() {
	return INC_zero::BYTES;
}

int INC_zero::getCycles() {
	return INC_zero::CYCLES;
}

int INC_zerox::getOpcode() {
	return INC_zerox::OPCODE;
}

int INC_zerox::getBytes() {
	return INC_zerox::BYTES;
}

int INC_zerox::getCycles() {
	return INC_zerox::CYCLES;
}

int INC_abs::getOpcode() {
	return INC_abs::OPCODE;
}

int INC_abs::getBytes() {
	return INC_abs::BYTES;
}

int INC_abs::getCycles() {
	return INC_abs::CYCLES;
}

int INC_absx::getOpcode() {
	return INC_absx::OPCODE;
}

int INC_absx::getBytes() {
	return INC_absx::BYTES;
}

int INC_absx::getCycles() {
	return INC_absx::CYCLES;
}

int INX::getOpcode() {
	return INX::OPCODE;
}

int INX::getBytes() {
	return INX::BYTES;
}

int INX::getCycles() {
	return INX::CYCLES;
}

int INY::getOpcode() {
	return INY::OPCODE;
}

int INY::getBytes() {
	return INY::BYTES;
}

int INY::getCycles() {
	return INY::CYCLES;
}

int JMP_abs::getOpcode() {
	return JMP_abs::OPCODE;
}

int JMP_abs::getBytes() {
	return JMP_abs::BYTES;
}

int JMP_abs::getCycles() {
	return JMP_abs::CYCLES;
}

int JMP_indirect::getOpcode() {
	return JMP_indirect::OPCODE;
}

int JMP_indirect::getBytes() {
	return JMP_indirect::BYTES;
}

int JMP_indirect::getCycles() {
	return JMP_indirect::CYCLES;
}

int JSR::getOpcode() {
	return JSR::OPCODE;
}

int JSR::getBytes() {
	return JSR::BYTES;
}

int JSR::getCycles() {
	return JSR::CYCLES;
}

int LDA_inmediate::getOpcode() {
	return LDA_inmediate::OPCODE;
}

int LDA_inmediate::getBytes() {
	return LDA_inmediate::BYTES;
}

int LDA_inmediate::getCycles() {
	return LDA_inmediate::CYCLES;
}

int LDA_zero::getOpcode() {
	return LDA_zero::OPCODE;
}

int LDA_zero::getBytes() {
	return LDA_zero::BYTES;
}

int LDA_zero::getCycles() {
	return LDA_zero::CYCLES;
}

int LDA_zerox::getOpcode() {
	return LDA_zerox::OPCODE;
}

int LDA_zerox::getBytes() {
	return LDA_zerox::BYTES;
}

int LDA_zerox::getCycles() {
	return LDA_zerox::CYCLES;
}

int LDA_abs::getOpcode() {
	return LDA_abs::OPCODE;
}

int LDA_abs::getBytes() {
	return LDA_abs::BYTES;
}

int LDA_abs::getCycles() {
	return LDA_abs::CYCLES;
}

int LDA_absx::getOpcode() {
	return LDA_absx::OPCODE;
}

int LDA_absx::getBytes() {
	return LDA_absx::BYTES;
}

int LDA_absx::getCycles() {
	return LDA_absx::CYCLES;
}

int LDA_absy::getOpcode() {
	return LDA_absy::OPCODE;
}

int LDA_absy::getBytes() {
	return LDA_absy::BYTES;
}

int LDA_absy::getCycles() {
	return LDA_absy::CYCLES;
}

int LDA_preindexi::getOpcode() {
	return LDA_preindexi::OPCODE;
}

int LDA_preindexi::getBytes() {
	return LDA_preindexi::BYTES;
}

int LDA_preindexi::getCycles() {
	return LDA_preindexi::CYCLES;
}

int LDA_postindexi::getOpcode() {
	return LDA_postindexi::OPCODE;
}

int LDA_postindexi::getBytes() {
	return LDA_postindexi::BYTES;
}

int LDA_postindexi::getCycles() {
	return LDA_postindexi::CYCLES;
}

int LDX_inmediate::getOpcode() {
	return LDX_inmediate::OPCODE;
}

int LDX_inmediate::getBytes() {
	return LDX_inmediate::BYTES;
}

int LDX_inmediate::getCycles() {
	return LDX_inmediate::CYCLES;
}

int LDX_zero::getOpcode() {
	return LDX_zero::OPCODE;
}

int LDX_zero::getBytes() {
	return LDX_zero::BYTES;
}

int LDX_zero::getCycles() {
	return LDX_zero::CYCLES;
}

int LDX_zeroy::getOpcode() {
	return LDX_zeroy::OPCODE;
}

int LDX_zeroy::getBytes() {
	return LDX_zeroy::BYTES;
}

int LDX_zeroy::getCycles() {
	return LDX_zeroy::CYCLES;
}

int LDX_abs::getOpcode() {
	return LDX_abs::OPCODE;
}

int LDX_abs::getBytes() {
	return LDX_abs::BYTES;
}

int LDX_abs::getCycles() {
	return LDX_abs::CYCLES;
}

int LDX_absy::getOpcode() {
	return LDX_absy::OPCODE;
}

int LDX_absy::getBytes() {
	return LDX_absy::BYTES;
}

int LDX_absy::getCycles() {
	return LDX_absy::CYCLES;
}

int LDY_inmediate::getOpcode() {
	return LDY_inmediate::OPCODE;
}

int LDY_inmediate::getBytes() {
	return LDY_inmediate::BYTES;
}

int LDY_inmediate::getCycles() {
	return LDY_inmediate::CYCLES;
}

int LDY_zero::getOpcode() {
	return LDY_zero::OPCODE;
}

int LDY_zero::getBytes() {
	return LDY_zero::BYTES;
}

int LDY_zero::getCycles() {
	return LDY_zero::CYCLES;
}

int LDY_zerox::getOpcode() {
	return LDY_zerox::OPCODE;
}

int LDY_zerox::getBytes() {
	return LDY_zerox::BYTES;
}

int LDY_zerox::getCycles() {
	return LDY_zerox::CYCLES;
}

int LDY_abs::getOpcode() {
	return LDY_abs::OPCODE;
}

int LDY_abs::getBytes() {
	return LDY_abs::BYTES;
}

int LDY_abs::getCycles() {
	return LDY_abs::CYCLES;
}

int LDY_absx::getOpcode() {
	return LDY_absx::OPCODE;
}

int LDY_absx::getBytes() {
	return LDY_absx::BYTES;
}

int LDY_absx::getCycles() {
	return LDY_absx::CYCLES;
}

int LSR_accumulator::getOpcode() {
	return LSR_accumulator::OPCODE;
}

int LSR_accumulator::getBytes() {
	return LSR_accumulator::BYTES;
}

int LSR_accumulator::getCycles() {
	return LSR_accumulator::CYCLES;
}

int LSR_zero::getOpcode() {
	return LSR_zero::OPCODE;
}

int LSR_zero::getBytes() {
	return LSR_zero::BYTES;
}

int LSR_zero::getCycles() {
	return LSR_zero::CYCLES;
}

int LSR_zerox::getOpcode() {
	return LSR_zerox::OPCODE;
}

int LSR_zerox::getBytes() {
	return LSR_zerox::BYTES;
}

int LSR_zerox::getCycles() {
	return LSR_zerox::CYCLES;
}

int LSR_abs::getOpcode() {
	return LSR_abs::OPCODE;
}

int LSR_abs::getBytes() {
	return LSR_abs::BYTES;
}

int LSR_abs::getCycles() {
	return LSR_abs::CYCLES;
}

int LSR_absx::getOpcode() {
	return LSR_absx::OPCODE;
}

int LSR_absx::getBytes() {
	return LSR_absx::BYTES;
}

int LSR_absx::getCycles() {
	return LSR_absx::CYCLES;
}

int NOP::getOpcode() {
	return NOP::OPCODE;
}

int NOP::getBytes() {
	return NOP::BYTES;
}

int NOP::getCycles() {
	return NOP::CYCLES;
}

int ORA_inmediate::getOpcode() {
	return ORA_inmediate::OPCODE;
}

int ORA_inmediate::getBytes() {
	return ORA_inmediate::BYTES;
}

int ORA_inmediate::getCycles() {
	return ORA_inmediate::CYCLES;
}

int ORA_zero::getOpcode() {
	return ORA_zero::OPCODE;
}

int ORA_zero::getBytes() {
	return ORA_zero::BYTES;
}

int ORA_zero::getCycles() {
	return ORA_zero::CYCLES;
}

int ORA_zerox::getOpcode() {
	return ORA_zerox::OPCODE;
}

int ORA_zerox::getBytes() {
	return ORA_zerox::BYTES;
}

int ORA_zerox::getCycles() {
	return ORA_zerox::CYCLES;
}

int ORA_abs::getOpcode() {
	return ORA_abs::OPCODE;
}

int ORA_abs::getBytes() {
	return ORA_abs::BYTES;
}

int ORA_abs::getCycles() {
	return ORA_abs::CYCLES;
}

int ORA_absx::getOpcode() {
	return ORA_absx::OPCODE;
}

int ORA_absx::getBytes() {
	return ORA_absx::BYTES;
}

int ORA_absx::getCycles() {
	return ORA_absx::CYCLES;
}

int ORA_absy::getOpcode() {
	return ORA_absy::OPCODE;
}

int ORA_absy::getBytes() {
	return ORA_absy::BYTES;
}

int ORA_absy::getCycles() {
	return ORA_absy::CYCLES;
}

int ORA_preindexi::getOpcode() {
	return ORA_preindexi::OPCODE;
}

int ORA_preindexi::getBytes() {
	return ORA_preindexi::BYTES;
}

int ORA_preindexi::getCycles() {
	return ORA_preindexi::CYCLES;
}

int ORA_postindexi::getOpcode() {
	return ORA_postindexi::OPCODE;
}

int ORA_postindexi::getBytes() {
	return ORA_postindexi::BYTES;
}

int ORA_postindexi::getCycles() {
	return ORA_postindexi::CYCLES;
}

int PHA::getOpcode() {
	return PHA::OPCODE;
}

int PHA::getBytes() {
	return PHA::BYTES;
}

int PHA::getCycles() {
	return PHA::CYCLES;
}

int PHP::getOpcode() {
	return PHP::OPCODE;
}

int PHP::getBytes() {
	return PHP::BYTES;
}

int PHP::getCycles() {
	return PHP::CYCLES;
}

int PLA::getOpcode() {
	return PLA::OPCODE;
}

int PLA::getBytes() {
	return PLA::BYTES;
}

int PLA::getCycles() {
	return PLA::CYCLES;
}

int PLP::getOpcode() {
	return PLP::OPCODE;
}

int PLP::getBytes() {
	return PLP::BYTES;
}

int PLP::getCycles() {
	return PLP::CYCLES;
}

int ROL_accumulator::getOpcode() {
	return ROL_accumulator::OPCODE;
}

int ROL_accumulator::getBytes() {
	return ROL_accumulator::BYTES;
}

int ROL_accumulator::getCycles() {
	return ROL_accumulator::CYCLES;
}

int ROL_zero::getOpcode() {
	return ROL_zero::OPCODE;
}

int ROL_zero::getBytes() {
	return ROL_zero::BYTES;
}

int ROL_zero::getCycles() {
	return ROL_zero::CYCLES;
}

int ROL_zerox::getOpcode() {
	return ROL_zerox::OPCODE;
}

int ROL_zerox::getBytes() {
	return ROL_zerox::BYTES;
}

int ROL_zerox::getCycles() {
	return ROL_zerox::CYCLES;
}

int ROL_abs::getOpcode() {
	return ROL_abs::OPCODE;
}

int ROL_abs::getBytes() {
	return ROL_abs::BYTES;
}

int ROL_abs::getCycles() {
	return ROL_abs::CYCLES;
}

int ROL_absx::getOpcode() {
	return ROL_absx::OPCODE;
}

int ROL_absx::getBytes() {
	return ROL_absx::BYTES;
}

int ROL_absx::getCycles() {
	return ROL_absx::CYCLES;
}

int ROR_accumulator::getOpcode() {
	return ROR_accumulator::OPCODE;
}

int ROR_accumulator::getBytes() {
	return ROR_accumulator::BYTES;
}

int ROR_accumulator::getCycles() {
	return ROR_accumulator::CYCLES;
}

int ROR_zero::getOpcode() {
	return ROR_zero::OPCODE;
}

int ROR_zero::getBytes() {
	return ROR_zero::BYTES;
}

int ROR_zero::getCycles() {
	return ROR_zero::CYCLES;
}

int ROR_zerox::getOpcode() {
	return ROR_zerox::OPCODE;
}

int ROR_zerox::getBytes() {
	return ROR_zerox::BYTES;
}

int ROR_zerox::getCycles() {
	return ROR_zerox::CYCLES;
}

int ROR_abs::getOpcode() {
	return ROR_abs::OPCODE;
}

int ROR_abs::getBytes() {
	return ROR_abs::BYTES;
}

int ROR_abs::getCycles() {
	return ROR_abs::CYCLES;
}

int ROR_absx::getOpcode() {
	return ROR_absx::OPCODE;
}

int ROR_absx::getBytes() {
	return ROR_absx::BYTES;
}

int ROR_absx::getCycles() {
	return ROR_absx::CYCLES;
}

int RTI::getOpcode() {
	return RTI::OPCODE;
}

int RTI::getBytes() {
	return RTI::BYTES;
}

int RTI::getCycles() {
	return RTI::CYCLES;
}

int RTS::getOpcode() {
	return RTS::OPCODE;
}

int RTS::getBytes() {
	return RTS::BYTES;
}

int RTS::getCycles() {
	return RTS::CYCLES;
}

int SBC_inmediate::getOpcode() {
	return SBC_inmediate::OPCODE;
}

int SBC_inmediate::getBytes() {
	return SBC_inmediate::BYTES;
}

int SBC_inmediate::getCycles() {
	return SBC_inmediate::CYCLES;
}

int SBC_zero::getOpcode() {
	return SBC_zero::OPCODE;
}

int SBC_zero::getBytes() {
	return SBC_zero::BYTES;
}

int SBC_zero::getCycles() {
	return SBC_zero::CYCLES;
}

int SBC_zerox::getOpcode() {
	return SBC_zerox::OPCODE;
}

int SBC_zerox::getBytes() {
	return SBC_zerox::BYTES;
}

int SBC_zerox::getCycles() {
	return SBC_zerox::CYCLES;
}

int SBC_abs::getOpcode() {
	return SBC_abs::OPCODE;
}

int SBC_abs::getBytes() {
	return SBC_abs::BYTES;
}

int SBC_abs::getCycles() {
	return SBC_abs::CYCLES;
}

int SBC_absx::getOpcode() {
	return SBC_absx::OPCODE;
}

int SBC_absx::getBytes() {
	return SBC_absx::BYTES;
}

int SBC_absx::getCycles() {
	return SBC_absx::CYCLES;
}

int SBC_absy::getOpcode() {
	return SBC_absy::OPCODE;
}

int SBC_absy::getBytes() {
	return SBC_absy::BYTES;
}

int SBC_absy::getCycles() {
	return SBC_absy::CYCLES;
}

int SBC_preindexi::getOpcode() {
	return SBC_preindexi::OPCODE;
}

int SBC_preindexi::getBytes() {
	return SBC_preindexi::BYTES;
}

int SBC_preindexi::getCycles() {
	return SBC_preindexi::CYCLES;
}

int SBC_postindexi::getOpcode() {
	return SBC_postindexi::OPCODE;
}

int SBC_postindexi::getBytes() {
	return SBC_postindexi::BYTES;
}

int SBC_postindexi::getCycles() {
	return SBC_postindexi::CYCLES;
}

int SEC::getOpcode() {
	return SEC::OPCODE;
}

int SEC::getBytes() {
	return SEC::BYTES;
}

int SEC::getCycles() {
	return SEC::CYCLES;
}

int SED::getOpcode() {
	return SED::OPCODE;
}

int SED::getBytes() {
	return SED::BYTES;
}

int SED::getCycles() {
	return SED::CYCLES;
}

int SEI::getOpcode() {
	return SEI::OPCODE;
}

int SEI::getBytes() {
	return SEI::BYTES;
}

int SEI::getCycles() {
	return SEI::CYCLES;
}

int STA_zero::getOpcode() {
	return STA_zero::OPCODE;
}

int STA_zero::getBytes() {
	return STA_zero::BYTES;
}

int STA_zero::getCycles() {
	return STA_zero::CYCLES;
}

int STA_zerox::getOpcode() {
	return STA_zerox::OPCODE;
}

int STA_zerox::getBytes() {
	return STA_zerox::BYTES;
}

int STA_zerox::getCycles() {
	return STA_zerox::CYCLES;
}

int STA_abs::getOpcode() {
	return STA_abs::OPCODE;
}

int STA_abs::getBytes() {
	return STA_abs::BYTES;
}

int STA_abs::getCycles() {
	return STA_abs::CYCLES;
}

int STA_absx::getOpcode() {
	return STA_absx::OPCODE;
}

int STA_absx::getBytes() {
	return STA_absx::BYTES;
}

int STA_absx::getCycles() {
	return STA_absx::CYCLES;
}

int STA_absy::getOpcode() {
	return STA_absy::OPCODE;
}

int STA_absy::getBytes() {
	return STA_absy::BYTES;
}

int STA_absy::getCycles() {
	return STA_absy::CYCLES;
}

int STA_preindexi::getOpcode() {
	return STA_preindexi::OPCODE;
}

int STA_preindexi::getBytes() {
	return STA_preindexi::BYTES;
}

int STA_preindexi::getCycles() {
	return STA_preindexi::CYCLES;
}

int STA_postindexi::getOpcode() {
	return STA_postindexi::OPCODE;
}

int STA_postindexi::getBytes() {
	return STA_postindexi::BYTES;
}

int STA_postindexi::getCycles() {
	return STA_postindexi::CYCLES;
}

int STX_zero::getOpcode() {
	return STX_zero::OPCODE;
}

int STX_zero::getBytes() {
	return STX_zero::BYTES;
}

int STX_zero::getCycles() {
	return STX_zero::CYCLES;
}

int STX_zeroy::getOpcode() {
	return STX_zeroy::OPCODE;
}

int STX_zeroy::getBytes() {
	return STX_zeroy::BYTES;
}

int STX_zeroy::getCycles() {
	return STX_zeroy::CYCLES;
}

int STX_abs::getOpcode() {
	return STX_abs::OPCODE;
}

int STX_abs::getBytes() {
	return STX_abs::BYTES;
}

int STX_abs::getCycles() {
	return STX_abs::CYCLES;
}

int STY_zero::getOpcode() {
	return STY_zero::OPCODE;
}

int STY_zero::getBytes() {
	return STY_zero::BYTES;
}

int STY_zero::getCycles() {
	return STY_zero::CYCLES;
}

int STY_zerox::getOpcode() {
	return STY_zerox::OPCODE;
}

int STY_zerox::getBytes() {
	return STY_zerox::BYTES;
}

int STY_zerox::getCycles() {
	return STY_zerox::CYCLES;
}

int STY_abs::getOpcode() {
	return STY_abs::OPCODE;
}

int STY_abs::getBytes() {
	return STY_abs::BYTES;
}

int STY_abs::getCycles() {
	return STY_abs::CYCLES;
}

int TAX::getOpcode() {
	return TAX::OPCODE;
}

int TAX::getBytes() {
	return TAX::BYTES;
}

int TAX::getCycles() {
	return TAX::CYCLES;
}

int TAY::getOpcode() {
	return TAY::OPCODE;
}

int TAY::getBytes() {
	return TAY::BYTES;
}

int TAY::getCycles() {
	return TAY::CYCLES;
}

int TSX::getOpcode() {
	return TSX::OPCODE;
}

int TSX::getBytes() {
	return TSX::BYTES;
}

int TSX::getCycles() {
	return TSX::CYCLES;
}

int TXA::getOpcode() {
	return TXA::OPCODE;
}

int TXA::getBytes() {
	return TXA::BYTES;
}

int TXA::getCycles() {
	return TXA::CYCLES;
}

int TXS::getOpcode() {
	return TXS::OPCODE;
}

int TXS::getBytes() {
	return TXS::BYTES;
}

int TXS::getCycles() {
	return TXS::CYCLES;
}

int TYA::getOpcode() {
	return TYA::OPCODE;
}

int TYA::getBytes() {
	return TYA::BYTES;
}

int TYA::getCycles() {
	return TYA::CYCLES;
}
