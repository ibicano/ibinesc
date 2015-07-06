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
	// Constantes
	static const int OPCODE = 0x69;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	ADC_inmediate(int operand, CPU* cpu);
	virtual ~ADC_inmediate();

	int execute();
};


class ADC_zero : public ADC {
	// Constantes
	static const int OPCODE = 0x65;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	ADC_zero(int operand, CPU* cpu);
	virtual ~ADC_zero();

	int execute();
};


class ADC_zerox : public ADC {
	// Constantes
	static const int OPCODE = 0x75;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	ADC_zerox(int operand, CPU* cpu);
	virtual ~ADC_zerox();

	int execute();
};


class ADC_abs : public ADC {
	// Constantes
	static const int OPCODE = 0x6D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_abs(int operand, CPU* cpu);
	virtual ~ADC_abs();

	int execute();
};


class ADC_absx : public ADC {
	// Constantes
	static const int OPCODE = 0x7D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_absx(int operand, CPU* cpu);
	virtual ~ADC_absx();

	int execute();
};


class ADC_absy : public ADC {
	// Constantes
	static const int OPCODE = 0x79;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	ADC_absy(int operand, CPU* cpu);
	virtual ~ADC_absy();

	int execute();
};


class ADC_preindexi : public ADC {
	// Constantes
	static const int OPCODE = 0x61;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	ADC_preindexi(int operand, CPU* cpu);
	virtual ~ADC_preindexi();

	int execute();
};


class ADC_postindexi : public ADC {
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
	// Constantes
	static const int OPCODE = 0x29;
	static const int BYTES = 2;
	static const int CYCLES = 2;

	AND_inmediate(int operand, CPU* cpu);
	virtual ~AND_inmediate();

	int execute();
};


class AND_zero : public AND {
	// Constantes
	static const int OPCODE = 0x25;
	static const int BYTES = 2;
	static const int CYCLES = 3;

	AND_zero(int operand, CPU* cpu);
	virtual ~AND_zero();

	int execute();
};


class AND_zerox : public AND {
	// Constantes
	static const int OPCODE = 0x35;
	static const int BYTES = 2;
	static const int CYCLES = 4;

	AND_zerox(int operand, CPU* cpu);
	virtual ~AND_zerox();

	int execute();
};


class AND_abs : public AND {
	// Constantes
	static const int OPCODE = 0x2D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_abs(int operand, CPU* cpu);
	virtual ~AND_abs();

	int execute();
};


class AND_absx : public AND {
	// Constantes
	static const int OPCODE = 0x3D;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_absx(int operand, CPU* cpu);
	virtual ~AND_absx();

	int execute();
};


class AND_absy : public AND {
	// Constantes
	static const int OPCODE = 0x39;
	static const int BYTES = 3;
	static const int CYCLES = 4;

	AND_absy(int operand, CPU* cpu);
	virtual ~AND_absy();

	int execute();
};


class AND_preindexi : public AND {
	// Constantes
	static const int OPCODE = 0x21;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	AND_preindexi(int operand, CPU* cpu);
	virtual ~AND_preindexi();

	int execute();
};


class AND_postindexi : public AND {
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
	// Constantes
	static const int OPCODE = 0x0A;
	static const int BYTES = 1;
	static const int CYCLES = 2;

	ASL_accumulator(int operand, CPU* cpu);
	virtual ~ASL_accumulator();

	int execute();
};


class ASL_zero : public ASL {
	// Constantes
	static const int OPCODE = 0x06;
	static const int BYTES = 2;
	static const int CYCLES = 5;

	ASL_zero(int operand, CPU* cpu);
	virtual ~ASL_zero();

	int execute();
};


class ASL_zerox : public ASL {
	// Constantes
	static const int OPCODE = 0x16;
	static const int BYTES = 2;
	static const int CYCLES = 6;

	ASL_zerox(int operand, CPU* cpu);
	virtual ~ASL_zerox();

	int execute();
};


class ASL_abs : public ASL {
	// Constantes
	static const int OPCODE = 0x0E;
	static const int BYTES = 3;
	static const int CYCLES = 6;

	ASL_abs(int operand, CPU* cpu);
	virtual ~ASL_abs();

	int execute();
};


class ASL_absx : public ASL {
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


#endif /* CPU_INSTRUCTION_HPP_ */
