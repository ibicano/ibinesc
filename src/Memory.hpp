/*
 * Memory.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

class Memory {
public:
	Memory();
	virtual ~Memory();

	// Lee una posición de memoria
	int readData(int addr);

	// Escribe un dato en una posición de memoria
	void writeData(int data, int addr);
};

#endif /* MEMORY_HPP_ */
