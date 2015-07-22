/*
 * Input.hpp
 *
 *  Created on: 3 de jul. de 2015
 *      Author: ibon
 */

#ifndef INPUT_HPP_
#define INPUT_HPP_

class Input {
public:
	Input();
	virtual ~Input();

private:
	int readCount;
	int write;
};


class Joypad : public Input {
public:
	Joypad();
	virtual ~Joypad();

	int readReg();			// Lee el registro del dispositivo
	void writeReg(int v);		// Escribe el registro del dispositivo


	// Lee y establece el valor de los botones
	bool getA();
	void setA(bool v);

	bool getB();
	void setB(bool v);

	bool getDown();
	void setDown(bool down);

	bool getLeft();
	void setLeft(bool v);

	bool getRight();
	void setRight(bool v);

	bool getSelect();
	void setSelect(bool v);

	bool getStart();
	void setStart(bool v);

	bool getUp();
	void setUp(bool v);

protected:
	int readCount;			// Contador del número de lectura del dispositivo
	int write;				// Almacena el valor del bit de la última escritura al registro de input

	// Variables que indican el estado de los controles
	bool up;
	bool down;
	bool left;
	bool right;
	bool a;
	bool b;
	bool select;
	bool start;
};


#endif /* INPUT_HPP_ */
