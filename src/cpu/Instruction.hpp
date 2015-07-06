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

	// Devuelve el operando de la instrucción
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


#endif /* CPU_INSTRUCTION_HPP_ */
