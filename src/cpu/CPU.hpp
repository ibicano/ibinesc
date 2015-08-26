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
#include "Instruction.hpp"

// Forwards para evitar el include
class PPU;

class CPU {
public:
	/*
	 * Constantes públicas
	 */

	// Posiciones en el registro de estado
	static const int REG_P_BIT_C = 0;
    static const int REG_P_BIT_Z = 1;
    static const int REG_P_BIT_I = 2;
    static const int REG_P_BIT_D = 3;
    static const int REG_P_BIT_B = 4;
    static const int REG_P_BIT_V = 6;
    static const int REG_P_BIT_S = 7;

    // Frecuencia de la CPU en Hz
    static const int CPU_FREQ = 1660000;

    // Direcciones de memoria vector de interrupciones
    static const int INT_ADDR_VBLANK = 0xFFFA;
    static const int INT_ADDR_RESET = 0xFFFC;
    static const int INT_ADDR_IRQ = 0xFFFE;

    // Latencia de interrupción en ciclos
    static const int INT_LATENCY = 7;

	/*
	 * Métodos públicos
	 */

	CPU(Memory* mem, PPU* ppu);
	virtual ~CPU();

	// Resetea el estado de la CPU
	void reset();

	// Devuelve un puntero a la memoria
	Memory* getMem();

	// Procesan interrupciones
	void interruptIrq();
	void interruptVblank();

	// Indica si hay una IRQ pendiente
	bool getIrq();
	void setIrq(bool v);

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
	int setCarryBit(int instResult);
	int setZeroBit(int instResult);
	int setSignBit(int instResult);

	// Mete y saca datos de la pila
	void pushStack(int byte);
	int pullStack();


private:
	/*
	 * Variables de instancia privadas
	 */

	Memory* mem;		// Memoria principal
	PPU* ppu;			// Procesador gráfico

	// Variables de los registros
	int regPc;
	int regSp;
	int regA;
	int regX;
	int regY;
	int regP;

	// Pool de instrucciones
	InstructionsPool* instructionsPool;

	// Flag de irq
	bool irq;

	/*
	 * Métodos de instancia privados
	 */

	// Procesa una interrupción
	void interrupt(int vectorAddr);

};// class CPU;

#endif /* CPU_CPU_HPP_ */
