/*
 * Instruction.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef CPU_INSTRUCTION_HPP_
#define CPU_INSTRUCTION_HPP_

// Forward para evitar el include
class CPU;

// Clase abstracte que sirve como base para las instrucciones
class Instruction {
public:
	// Constantes
	static const int OPCODE = 0;
	static const int BYTES = 0;
	static const int CYCLES = 0;

	// Métodos públicos
	Instruction(int operand, CPU* cpu);

	virtual ~Instruction() = 0;

	// Ejecuta la instrucción
	virtual int execute() = 0;

	// Devuelve el número de ciclos que consume la instrucción
	virtual void getCycles() = 0;

	// Devuelve el operand de la instrucción
	virtual void getOperand() = 0;

	// Establece el oprando de la instrucción
	virtual void setOperand() = 0;

protected:
	int operand;
	CPU* cpu;

	// Funciones que calculan la dirección de los modos de direccionamiento
	int fetchAbsoluteAddrmode();
	int fetchIndexedAbsXAddrmode();
	int fetchIndexedAbsYAddrmode();
	int fetchIndexedZeroXAddrmode();
	int fetchIndexedZeroYAddrmode();
	int fetchPreindexedAddrmode();
	int fetchPostindexedAddrmode();

};//class Instruction


/******************************************************************************
 * ADC: Add memory to accumulator with carry
 *****************************************************************************/
class ADC : public Instruction {
public:
	ADC(int operand, CPU* cpu);
	virtual ~ADC();

	virtual int execute(int op);

};//class ADC


class ADC_inmediate : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x69;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	ADC_inmediate(int operand, CPU* cpu);
	virtual ~ADC_inmediate();

	int execute();
};


class ADC_zero : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x65;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	ADC_zero(int operand, CPU* cpu);
	virtual ~ADC_zero();

	int execute();
};


class ADC_zerox : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x75;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	ADC_zerox(int operand, CPU* cpu);
	virtual ~ADC_zerox();

	int execute();
};


class ADC_abs : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x6D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_abs(int operand, CPU* cpu);
	virtual ~ADC_abs();

	int execute();
};


class ADC_absx : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x7D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_absx(int operand, CPU* cpu);
	virtual ~ADC_absx();

	int execute();
};


class ADC_absy : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x79;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_absy(int operand, CPU* cpu);
	virtual ~ADC_absy();

	int execute();
};


class ADC_preindexi : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x61;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	ADC_preindexi(int operand, CPU* cpu);
	virtual ~ADC_preindexi();

	int execute();
};


class ADC_postindexi : public ADC {
public:
	// Constantes
	static const int OPCODE = 0x71;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	ADC_postindexi(int operand, CPU* cpu);
	virtual ~ADC_postindexi();

	int execute();
};


/******************************************************************************
 * AND: And memory with accumulator
 *****************************************************************************/
class AND : public Instruction {
public:
	AND(int operand, CPU* cpu);
	virtual ~AND();

	virtual int execute(int op);

};//class AND


class AND_inmediate : public AND {
public:
	// Constantes
	static const int OPCODE = 0x29;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	AND_inmediate(int operand, CPU* cpu);
	virtual ~AND_inmediate();

	int execute();
};


class AND_zero : public AND {
public:
	// Constantes
	static const int OPCODE = 0x25;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	AND_zero(int operand, CPU* cpu);
	virtual ~AND_zero();

	int execute();
};


class AND_zerox : public AND {
public:
	// Constantes
	static const int OPCODE = 0x35;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	AND_zerox(int operand, CPU* cpu);
	virtual ~AND_zerox();

	int execute();
};


class AND_abs : public AND {
public:
	// Constantes
	static const int OPCODE = 0x2D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_abs(int operand, CPU* cpu);
	virtual ~AND_abs();

	int execute();
};


class AND_absx : public AND {
public:
	// Constantes
	static const int OPCODE = 0x3D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_absx(int operand, CPU* cpu);
	virtual ~AND_absx();

	int execute();
};


class AND_absy : public AND {
public:
	// Constantes
	static const int OPCODE = 0x39;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_absy(int operand, CPU* cpu);
	virtual ~AND_absy();

	int execute();
};


class AND_preindexi : public AND {
public:
	// Constantes
	static const int OPCODE = 0x21;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	AND_preindexi(int operand, CPU* cpu);
	virtual ~AND_preindexi();

	int execute();
};


class AND_postindexi : public AND {
public:
	// Constantes
	static const int OPCODE = 0x31;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	AND_postindexi(int operand, CPU* cpu);
	virtual ~AND_postindexi();

	int execute();
};


/******************************************************************************
 * ASL: Shift Left One Bit (Memory or Accumulator)
 *****************************************************************************/
class ASL : public Instruction {
public:
	ASL(int operand, CPU* cpu);
	virtual ~ASL();

	virtual int execute(int op);

};//class ASL


class ASL_accumulator : public ASL {
public:
	// Constantes
	static const int OPCODE = 0x0A;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	ASL_accumulator(int operand, CPU* cpu);
	virtual ~ASL_accumulator();

	int execute();
};


class ASL_zero : public ASL {
public:
	// Constantes
	static const int OPCODE = 0x06;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	ASL_zero(int operand, CPU* cpu);
	virtual ~ASL_zero();

	int execute();
};


class ASL_zerox : public ASL {
public:
	// Constantes
	static const int OPCODE = 0x16;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	ASL_zerox(int operand, CPU* cpu);
	virtual ~ASL_zerox();

	int execute();
};


class ASL_abs : public ASL {
public:
	// Constantes
	static const int OPCODE = 0x0E;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	ASL_abs(int operand, CPU* cpu);
	virtual ~ASL_abs();

	int execute();
};


class ASL_absx : public ASL {
public:
	// Constantes
	static const int OPCODE = 0x1E;
	static const int BYTES = 3;
	static const int CYCLES = 7;

	ASL_absx(int operand, CPU* cpu);
	virtual ~ASL_absx();

	int execute();
};


/******************************************************************************
 * BCC Branch on Carry Clear
 *****************************************************************************/
class BCC : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x90;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BCC(int operand, CPU* cpu);
	~BCC();

	int execute();
};//class BCC


/******************************************************************************
 * BCS Branch on carry set
 *****************************************************************************/
class BCS : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xB0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BCS(int operand, CPU* cpu);
	~BCS();

	int execute();
};//class BCS


/******************************************************************************
 * BEQ Branch on result zero
 *****************************************************************************/
class BEQ : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xF0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BEQ(int operand, CPU* cpu);
	~BEQ();

	int execute();
};//class BEQ


/******************************************************************************
 * BIT Test bits in memory with accumulator
 *****************************************************************************/
class BIT : public Instruction {
public:
	BIT(int operand, CPU* cpu);
	virtual ~BIT();

	virtual int execute();

};//class BIT


class BIT_zero : public BIT {
public:
	// Constantes
	static const int OPCODE = 0x24;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	BIT_zero(int operand, CPU* cpu);
	virtual ~BIT_zero();
};//class BIT_zero


class BIT_abs : public BIT {
public:
	// Constantes
	static const int OPCODE = 0x2C;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	BIT_abs(int operand, CPU* cpu);
	virtual ~BIT_abs();
};//class BIT_abs


/******************************************************************************
 * BMI Branch on result minus
 *****************************************************************************/
class BMI : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x30;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BMI(int operand, CPU* cpu);
	~BMI();

	int execute();
};//class BMI


/******************************************************************************
 * BNE Branch on result not zero
 *****************************************************************************/
class BNE : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xD0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BNE(int operand, CPU* cpu);
	~BNE();

	int execute();
};//class BNE


/******************************************************************************
 * BPL Branch on result plus
 *****************************************************************************/
class BPL : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x10;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BPL(int operand, CPU* cpu);
	~BPL();

	int execute();
};//class BPL


/******************************************************************************
 * BRK Force Break
 *****************************************************************************/
class BRK : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x00;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	BRK(CPU* cpu);
	~BRK();

	int execute();
};//class BRK


/******************************************************************************
 * BVC Branch on overflow clear
 *****************************************************************************/
class BVC : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x50;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BVC(int operand, CPU* cpu);
	~BVC();

	int execute();
};//class BVC


/******************************************************************************
 * BVS Branch on overflow set
 *****************************************************************************/
class BVS : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x70;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	BVS(int operand, CPU* cpu);
	~BVS();

	int execute();
};//class BVS


/******************************************************************************
 * CLC Clear carry flag
 *****************************************************************************/
class CLC : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x18;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	CLC(CPU* cpu);
	~CLC();

	int execute();
};//class CLC


/******************************************************************************
 * CLD Clear decimal mode
 *****************************************************************************/
class CLD : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xD8;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	CLD(CPU* cpu);
	~CLD();

	int execute();
};//class CLD


/******************************************************************************
 * CLI Clear interrupt disable bit
 *****************************************************************************/
class CLI : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x58;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	CLI(CPU* cpu);
	~CLI();

	int execute();
};//class CLI


/******************************************************************************
 * CLV Clear overflow flag
 *****************************************************************************/
class CLV : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xB8;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	CLV(CPU* cpu);
	~CLV();

	int execute();
};//class CLV


/******************************************************************************
 * CMP Compare memory and accumulator
 *****************************************************************************/
class CMP : public Instruction {
public:
	CMP(int operand, CPU* cpu);
	virtual ~CMP();

	virtual int execute(int op);

};//class CMP


class CMP_inmediate : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xC9;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	CMP_inmediate(int operand, CPU* cpu);
	virtual ~CMP_inmediate();

	int execute();
};


class CMP_zero : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xC5;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	CMP_zero(int operand, CPU* cpu);
	virtual ~CMP_zero();

	int execute();
};


class CMP_zerox : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xD5;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	CMP_zerox(int operand, CPU* cpu);
	virtual ~CMP_zerox();

	int execute();
};


class CMP_abs : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xCD;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	CMP_abs(int operand, CPU* cpu);
	virtual ~CMP_abs();

	int execute();
};


class CMP_absx : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xDD;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	CMP_absx(int operand, CPU* cpu);
	virtual ~CMP_absx();

	int execute();
};


class CMP_absy : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xD9;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	CMP_absy(int operand, CPU* cpu);
	virtual ~CMP_absy();

	int execute();
};


class CMP_preindexi : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xC1;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	CMP_preindexi(int operand, CPU* cpu);
	virtual ~CMP_preindexi();

	int execute();
};


class CMP_postindexi : public CMP {
public:
	// Constantes
	static const int OPCODE = 0xD1;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	CMP_postindexi(int operand, CPU* cpu);
	virtual ~CMP_postindexi();

	int execute();
};


/******************************************************************************
 * CPX Compare Memory and Index X
 *****************************************************************************/
class CPX : public Instruction {
public:
	CPX(int operand, CPU* cpu);
	virtual ~CPX();

	virtual int execute(int op);

};//class CPX


class CPX_inmediate : public CPX {
public:
	// Constantes
	static const int OPCODE = 0xE0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	CPX_inmediate(int operand, CPU* cpu);
	virtual ~CPX_inmediate();

	int execute();
};


class CPX_zero : public CPX {
public:
	// Constantes
	static const int OPCODE = 0xE4;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	CPX_zero(int operand, CPU* cpu);
	virtual ~CPX_zero();

	int execute();
};


class CPX_abs : public CPX {
public:
	// Constantes
	static const int OPCODE = 0xEC;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	CPX_abs(int operand, CPU* cpu);
	virtual ~CPX_abs();

	int execute();
};


/******************************************************************************
 * CPY Compare Memory and Index Y
 *****************************************************************************/
class CPY : public Instruction {
public:
	CPY(int operand, CPU* cpu);
	virtual ~CPY();

	virtual int execute(int op);

};//class CPY


class CPY_inmediate : public CPY {
public:
	// Constantes
	static const int OPCODE = 0xC0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	CPY_inmediate(int operand, CPU* cpu);
	virtual ~CPY_inmediate();

	int execute();
};


class CPY_zero : public CPY {
public:
	// Constantes
	static const int OPCODE = 0xC4;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	CPY_zero(int operand, CPU* cpu);
	virtual ~CPY_zero();

	int execute();
};


class CPY_abs : public CPY {
public:
	// Constantes
	static const int OPCODE = 0xCC;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	CPY_abs(int operand, CPU* cpu);
	virtual ~CPY_abs();

	int execute();
};


/******************************************************************************
 * DEC Decrement memory by one
 *****************************************************************************/
class DEC : public Instruction {
public:
	DEC(int operand, CPU* cpu);
	virtual ~DEC();

	virtual int execute(int op);

};//class DEC



class DEC_zero : public DEC {
public:
	// Constantes
	static const int OPCODE = 0xC6;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	DEC_zero(int operand, CPU* cpu);
	virtual ~DEC_zero();

	int execute();
};


class DEC_zerox : public DEC {
public:
	// Constantes
	static const int OPCODE = 0xD6;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	DEC_zerox(int operand, CPU* cpu);
	virtual ~DEC_zerox();

	int execute();
};


class DEC_abs : public DEC {
public:
	// Constantes
	static const int OPCODE = 0xCE;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	DEC_abs(int operand, CPU* cpu);
	virtual ~DEC_abs();

	int execute();
};


class DEC_absx : public DEC {
public:
	// Constantes
	static const int OPCODE = 0xDE;
	static const int BYTES = 3;
	static const int CYCLES = 7;

	DEC_absx(int operand, CPU* cpu);
	virtual ~DEC_absx();

	int execute();
};


/******************************************************************************
 * DEX Decrement index X by one
 *****************************************************************************/

class DEX : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xCA;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	DEX(CPU* cpu);
	virtual ~DEX();

	int execute();
};


/******************************************************************************
 * DEY Decrement index Y by one
 *****************************************************************************/

class DEY : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x88;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	DEY(CPU* cpu);
	virtual ~DEY();

	int execute();
};


/******************************************************************************
 * EOR "Exclusive-Or" memory with accumulator
 *****************************************************************************/
class EOR : public Instruction {
public:
	EOR(int operand, CPU* cpu);
	virtual ~EOR();

	virtual int execute(int op);

};//class EOR


class EOR_inmediate : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x49;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	EOR_inmediate(int operand, CPU* cpu);
	virtual ~EOR_inmediate();

	int execute();
};


class EOR_zero : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x45;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	EOR_zero(int operand, CPU* cpu);
	virtual ~EOR_zero();

	int execute();
};


class EOR_zerox : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x55;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	EOR_zerox(int operand, CPU* cpu);
	virtual ~EOR_zerox();

	int execute();
};


class EOR_abs : public EOR {
	// Constantes
	static const int OPCODE = 0x4D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	EOR_abs(int operand, CPU* cpu);
	virtual ~EOR_abs();

	int execute();
};


class EOR_absx : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x5D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	EOR_absx(int operand, CPU* cpu);
	virtual ~EOR_absx();

	int execute();
};


class EOR_absy : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x59;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	EOR_absy(int operand, CPU* cpu);
	virtual ~EOR_absy();

	int execute();
};


class EOR_preindexi : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x41;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	EOR_preindexi(int operand, CPU* cpu);
	virtual ~EOR_preindexi();

	int execute();
};


class EOR_postindexi : public EOR {
public:
	// Constantes
	static const int OPCODE = 0x51;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	EOR_postindexi(int operand, CPU* cpu);
	virtual ~EOR_postindexi();

	int execute();
};


/******************************************************************************
 * INC Increment memory by one
 *****************************************************************************/
class INC : public Instruction {
public:
	INC(int operand, CPU* cpu);
	virtual ~INC();

	virtual int execute(int op);

};//class INC



class INC_zero : public INC {
public:
	// Constantes
	static const int OPCODE = 0xE6;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	INC_zero(int operand, CPU* cpu);
	virtual ~INC_zero();

	int execute();
};


class INC_zerox : public INC {
	// Constantes
	static const int OPCODE = 0xF6;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	INC_zerox(int operand, CPU* cpu);
	virtual ~INC_zerox();

	int execute();
};


class INC_abs : public INC {
public:
	// Constantes
	static const int OPCODE = 0xEE;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	INC_abs(int operand, CPU* cpu);
	virtual ~INC_abs();

	int execute();
};


class INC_absx : public INC {
public:
	// Constantes
	static const int OPCODE = 0xFE;
	static const int BYTES = 3;
	static const int CYCLES = 7;

	INC_absx(int operand, CPU* cpu);
	virtual ~INC_absx();

	int execute();
};


/******************************************************************************
 * INX Increment Index X by one
 *****************************************************************************/

class INX : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xE8;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	INX(CPU* cpu);
	virtual ~INX();

	int execute();
};


/******************************************************************************
 * INY Increment Index Y by one
 *****************************************************************************/

class INY : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xC8;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	INY(CPU* cpu);
	virtual ~INY();

	int execute();
};


/******************************************************************************
 * JMP Jump to new location
 *****************************************************************************/
class JMP : public Instruction {
public:
	JMP(int operand, CPU* cpu);
	virtual ~JMP();

	virtual int execute(int op);

};//class JMP


class JMP_abs : public JMP {
public:
	// Constantes
	static const int OPCODE = 0x4C;
	static const int BYTES = 3;
	static const int CYCLES = 3;

	JMP_abs(int operand, CPU* cpu);
	virtual ~JMP_abs();

	int execute();
};


class JMP_indirect : public JMP {
public:
	// Constantes
	static const int OPCODE = 0x6C;
	static const int BYTES = 3;
	static const int CYCLES = 5;

	JMP_indirect(int operand, CPU* cpu);
	virtual ~JMP_indirect();

	int execute();
};


/******************************************************************************
 * JSR Jump to new location saving return address
 *****************************************************************************/
class JSR : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0x20;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	JSR(int operand, CPU* cpu);
	virtual ~JSR();

	virtual int execute(int op);

};//class JSR


/******************************************************************************
 * LDA Load accumulator with memory
 *****************************************************************************/
class LDA : public Instruction {
public:
	LDA(int operand, CPU* cpu);
	virtual ~LDA();

	virtual int execute(int op);

};//class LDA


class LDA_inmediate : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xA9;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	LDA_inmediate(int operand, CPU* cpu);
	virtual ~LDA_inmediate();

	int execute();
};


class LDA_zero : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xA5;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	LDA_zero(int operand, CPU* cpu);
	virtual ~LDA_zero();

	int execute();
};


class LDA_zerox : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xB5;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	LDA_zerox(int operand, CPU* cpu);
	virtual ~LDA_zerox();

	int execute();
};


class LDA_abs : public LDA {
	// Constantes
	static const int OPCODE = 0xAD;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDA_abs(int operand, CPU* cpu);
	virtual ~LDA_abs();

	int execute();
};


class LDA_absx : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xBD;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDA_absx(int operand, CPU* cpu);
	virtual ~LDA_absx();

	int execute();
};


class LDA_absy : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xB9;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDA_absy(int operand, CPU* cpu);
	virtual ~LDA_absy();

	int execute();
};


class LDA_preindexi : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xA1;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	LDA_preindexi(int operand, CPU* cpu);
	virtual ~LDA_preindexi();

	int execute();
};


class LDA_postindexi : public LDA {
public:
	// Constantes
	static const int OPCODE = 0xB1;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	LDA_postindexi(int operand, CPU* cpu);
	virtual ~LDA_postindexi();

	int execute();
};


/******************************************************************************
 * LDX Load index X with memory
 *****************************************************************************/
class LDX : public Instruction {
public:
	LDX(int operand, CPU* cpu);
	virtual ~LDX();

	virtual int execute(int op);

};//class LDX


class LDX_inmediate : public LDX {
public:
	// Constantes
	static const int OPCODE = 0xA2;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	LDX_inmediate(int operand, CPU* cpu);
	virtual ~LDX_inmediate();

	int execute();
};


class LDX_zero : public LDX {
public:
	// Constantes
	static const int OPCODE = 0xA6;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	LDX_zero(int operand, CPU* cpu);
	virtual ~LDX_zero();

	int execute();
};


class LDX_zeroy : public LDX {
public:
	// Constantes
	static const int OPCODE = 0xB6;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	LDX_zeroy(int operand, CPU* cpu);
	virtual ~LDX_zeroy();

	int execute();
};


class LDX_abs : public LDX {
	// Constantes
	static const int OPCODE = 0xAE;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDX_abs(int operand, CPU* cpu);
	virtual ~LDX_abs();

	int execute();
};


class LDX_absy : public LDX {
public:
	// Constantes
	static const int OPCODE = 0xBE;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDX_absy(int operand, CPU* cpu);
	virtual ~LDX_absy();

	int execute();
};


/******************************************************************************
 * LDY Load index Y with memory
 *****************************************************************************/
class LDY : public Instruction {
public:
	LDY(int operand, CPU* cpu);
	virtual ~LDY();

	virtual int execute(int op);

};//class LDY


class LDY_inmediate : public LDY {
public:
	// Constantes
	static const int OPCODE = 0xA0;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	LDY_inmediate(int operand, CPU* cpu);
	virtual ~LDY_inmediate();

	int execute();
};


class LDY_zero : public LDY {
public:
	// Constantes
	static const int OPCODE = 0xA4;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	LDY_zero(int operand, CPU* cpu);
	virtual ~LDY_zero();

	int execute();
};


class LDY_zerox : public LDY {
public:
	// Constantes
	static const int OPCODE = 0xB4;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	LDY_zerox(int operand, CPU* cpu);
	virtual ~LDY_zerox();

	int execute();
};


class LDY_abs : public LDY {
	// Constantes
	static const int OPCODE = 0xAC;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDY_abs(int operand, CPU* cpu);
	virtual ~LDY_abs();

	int execute();
};


class LDY_absx : public LDY {
public:
	// Constantes
	static const int OPCODE = 0xBC;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	LDY_absx(int operand, CPU* cpu);
	virtual ~LDY_absx();

	int execute();
};


/******************************************************************************
 * LSR Shift right one bit (memory or accumulator)
 *****************************************************************************/
class LSR : public Instruction {
public:
	LSR(int operand, CPU* cpu);
	virtual ~LSR();

	virtual int execute(int op);

};//class LSR


class LSR_accumulator : public LSR {
public:
	// Constantes
	static const int OPCODE = 0x4A;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	LSR_accumulator(int operand, CPU* cpu);
	virtual ~LSR_accumulator();

	int execute();
};


class LSR_zero : public LSR {
public:
	// Constantes
	static const int OPCODE = 0x46;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	LSR_zero(int operand, CPU* cpu);
	virtual ~LSR_zero();

	int execute();
};


class LSR_zerox : public LSR {
public:
	// Constantes
	static const int OPCODE = 0x56;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	LSR_zerox(int operand, CPU* cpu);
	virtual ~LSR_zerox();

	int execute();
};


class LSR_abs : public LSR {
public:
	// Constantes
	static const int OPCODE = 0x4E;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	LSR_abs(int operand, CPU* cpu);
	virtual ~LSR_abs();

	int execute();
};


class LSR_absx : public LSR {
public:
	// Constantes
	static const int OPCODE = 0x5E;
	static const int BYTES = 3;
	static const int CYCLES = 7;

	LSR_absx(int operand, CPU* cpu);
	virtual ~LSR_absx();

	int execute();
};


/******************************************************************************
 * NOP No operation
 *****************************************************************************/
class NOP : public Instruction {
public:
	// Constantes
	static const int OPCODE = 0xEA;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	NOP(CPU* cpu);
	virtual ~NOP();

	virtual int execute();

};//class JSR


/******************************************************************************
 * ORA "OR" memory with accumulator
 *****************************************************************************/
class ORA : public Instruction {
public:
	ORA(int operand, CPU* cpu);
	virtual ~ORA();

	virtual int execute(int op);

};//class ORA


class ORA_inmediate : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x09;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	ORA_inmediate(int operand, CPU* cpu);
	virtual ~ORA_inmediate();

	int execute();
};


class ORA_zero : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x05;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	ORA_zero(int operand, CPU* cpu);
	virtual ~ORA_zero();

	int execute();
};


class ORA_zerox : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x15;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	ORA_zerox(int operand, CPU* cpu);
	virtual ~ORA_zerox();

	int execute();
};


class ORA_abs : public ORA {
	// Constantes
	static const int OPCODE = 0x0D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ORA_abs(int operand, CPU* cpu);
	virtual ~ORA_abs();

	int execute();
};


class ORA_absx : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x1D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ORA_absx(int operand, CPU* cpu);
	virtual ~ORA_absx();

	int execute();
};


class ORA_absy : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x19;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ORA_absy(int operand, CPU* cpu);
	virtual ~ORA_absy();

	int execute();
};


class ORA_preindexi : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x01;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	ORA_preindexi(int operand, CPU* cpu);
	virtual ~ORA_preindexi();

	int execute();
};


class ORA_postindexi : public ORA {
public:
	// Constantes
	static const int OPCODE = 0x11;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	ORA_postindexi(int operand, CPU* cpu);
	virtual ~ORA_postindexi();

	int execute();
};



#endif /* CPU_INSTRUCTION_HPP_ */
