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
};


class Joypad : public Input {
public:
	Joypad();
	virtual ~Joypad();

	int readReg();			// Lee el registro del dispositivo
	void writeReg();		// Escribe el registro del dispositivo


	// Lee y establece el valor de los botones
	bool isA();
	void setA(bool a);

	bool isB();
	void setB(bool b);

	bool isDown();
	void setDown(bool down);

	bool isLeft();
	void setLeft(bool left);

	bool isRight();
	void setRight(bool right);

	bool isSelect();
	void setSelect(bool select);

	bool isStart();
	void setStart(bool start);

	bool isUp();
	void setUp(bool up);

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
