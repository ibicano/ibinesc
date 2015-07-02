/*
 * Instruction.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef CPU_INSTRUCTION_HPP_
#define CPU_INSTRUCTION_HPP_

// Clase abstracte que sirve como base para las instrucciones
class Instruction {
public:
	virtual ~Instruction() = 0;

	// Ejecuta la instrucción
	virtual void execute() = 0;

	// Devuelve el número de ciclos que consume la instrucción
	virtual void get_cycles() = 0;

	// Devuelve el operando de la instrucción
	virtual void get_operand() = 0;

	// Establece el oprando de la instrucción
	virtual void set_operand() = 0;

protected:
	// Funciones que calculan la dirección de los modos de direccionamiento
	int fetch_absolute_addrmode();
	int fetch_indexed_abs_x_addrmode();
	int fetch_indexed_abs_y_addrmode();
	int fetch_indexed_zero_x_addrmode();
	int fetch_indexed_zero_y_addrmode();
	int fetch_preindexed_addrmode();
	int fetch_postindexed_addrmode();
};

#endif /* CPU_INSTRUCTION_HPP_ */
