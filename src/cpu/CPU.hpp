/*
 * CPU.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef CPU_CPU_HPP_
#define CPU_CPU_HPP_

#include "../Memory.hpp"
#include "Instruction.hpp"

class CPU {
public:
	CPU();
	virtual ~CPU();

	// Procesan interrupciones
	void interrupt_irq();
	void interrupt_vblank();

	// Indica si hay una IRQ pendiente
	bool get_irq();
	bool set_irq(bool v);

	// Devuelve un puntero a la memoria
	Memory* get_mem();

	// Devuelven el contenido de los registros
	int get_reg_pc();
	int get_reg_sp();
	int get_reg_x();
	int get_reg_y();
	int get_reg_p();

	// Escriben el contenido de los registros
	void set_reg_pc(int v);
	void set_reg_sp(int v);
	void set_reg_x(int v);
	void set_reg_y(int v);
	void set_reg_p(int v);

	// Incrementa el registro PC en 'n' unidades
	void incr_pc(int n);

	// Devuelve la siguiente instrucción a ejecutar
	Instruction* fetch_inst();

	// Lee el valor de los campos del registro de estado
	int get_reg_p_c_bit();
	int get_reg_p_z_bit();
	int get_reg_p_i_bit();
	int get_reg_p_d_bit();
	int get_reg_p_b_bit();
	int get_reg_p_v_bit();
	int get_reg_p_s_bit();

	// Escribe el valor de los campos del registro de estado
	void set_reg_p_c_bit(int v);
	void set_reg_p_z_bit(int v);
	void set_reg_p_i_bit(int v);
	void set_reg_p_d_bit(int v);
	void set_reg_p_b_bit(int v);
	void set_reg_p_v_bit(int v);
	void set_reg_p_s_bit(int v);

	/* Calcula el valor de los bits del registro de estado en función
	 * del resultado de una instrucción */
	void set_carry_bit(int result);
	void set_zero_bit(int result);
	void set_sign_bit(int result);

	// Mete y saca datos de la pila
	void push_stack(int byte);
	int pull_stack();


private:
	// Procesa una interrupción
	void interrupt();
};

#endif /* CPU_CPU_HPP_ */
