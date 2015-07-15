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

int Joypad::readReg() {
}

void Joypad::writeReg(int data) {
}

bool Joypad::isA() {
}

void Joypad::setA(bool a) {
}

bool Joypad::isB() {
}

void Joypad::setB(bool b) {
}

bool Joypad::isDown() {
}

void Joypad::setDown(bool down) {
}

bool Joypad::isLeft() {
}

void Joypad::setLeft(bool left) {
}

bool Joypad::isRight() {
}

void Joypad::setRight(bool right) {
}

bool Joypad::isSelect() {
}

void Joypad::setSelect(bool select) {
}

bool Joypad::isStart() {
}

void Joypad::setStart(bool start) {
}

bool Joypad::isUp() {
}

void Joypad::setUp(bool up) {
}
