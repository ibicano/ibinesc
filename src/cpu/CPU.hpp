/*
 * CPU.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef CPU_CPU_HPP_
#define CPU_CPU_HPP_

#include "../Memory.hpp"
#include <iostream>

// Forward para evitar el include
class Instruction;

class CPU {
public:
	CPU(Memory* mem, PPU* ppu);
	virtual ~CPU();

	// Procesan interrupciones
	void interruptIrq();
	void interruptVblank();

	// Indica si hay una IRQ pendiente
	bool getIrq();
	bool setIrq(bool v);

	// Devuelve un puntero a la memoria
	Memory* getMem();

	// Devuelven el contenido de los registros
	int getRegPc();
	int getRegSp();
	int getRegA();
	int getRegX();
	int getRegY();
	int getRegP();

	// Escriben el contenido de los registros
	void setRegPc(int v);
	void setRegSp(int v);
	void setRegA(int v);
	void setRegX(int v);
	void setRegY(int v);
	void setRegP(int v);

	// Incrementa el registro PC en 'n' unidades
	void incrPc(int n);

	// Devuelve la siguiente instrucción a ejecutar
	Instruction* fetchInst();

	// Lee el valor de los campos del registro de estado
	int getReg_p_c_bit();
	int getReg_p_z_bit();
	int getReg_p_i_bit();
	int getReg_p_d_bit();
	int getReg_p_b_bit();
	int getReg_p_v_bit();
	int getReg_p_s_bit();

	// Escribe el valor de los campos del registro de estado
	void setReg_p_c_bit(int v);
	void setReg_p_z_bit(int v);
	void setReg_p_i_bit(int v);
	void setReg_p_d_bit(int v);
	void setReg_p_b_bit(int v);
	void setReg_p_v_bit(int v);
	void setReg_p_s_bit(int v);

	/* Calcula el valor de los bits del registro de estado en función
	 * del resultado de una instrucción */
	void setCarryBit(int result);
	void setZeroBit(int result);
	void setSignBit(int result);

	// Mete y saca datos de la pila
	void pushStack(int byte);
	int pullStack();


private:
	// Variables de instancia
	Memory* mem;		// Memoria principal
	PPU* ppu;			// Procesador gráfico

	// Variables de los registros
	int regPc;
	int regSp;
	int regA;
	int regX;
	int regY;
	int regP;

	// Procesa una interrupción
	void interrupt();

};// class CPU;

#endif /* CPU_CPU_HPP_ */
