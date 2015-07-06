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

	int execute(int op);

};//class ADC


#endif /* CPU_INSTRUCTION_HPP_ */
