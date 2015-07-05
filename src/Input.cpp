/*
 * Input.cpp
 *
 *  Created on: 3 de jul. de 2015
 *      Author: ibon
 */

#include "Input.hpp"

Input::Input() {

}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

Joypad::Joypad() {
	readCount = 0;
	write = 0;

	up = false;
	down = false;
	left = false;
	right = false;
	a = false;
	b = false;
	select = false;
	start = false;
}

Joypad::~Joypad() {
}
