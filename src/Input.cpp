/*
 * Input.cpp
 *
 *  Created on: 3 de jul. de 2015
 *      Author: ibon
 */

#include "Input.hpp"

Input::Input() {
	readCount = 0;
	write = 0;
}

Input::~Input() {
}

Joypad::Joypad() : Input(){
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
	bool v;

    if (readCount == 0)
        v = a;
    else if (readCount == 1)
        v = b;
    else if (readCount == 2)
        v = select;
    else if (readCount == 3)
        v = start;
    else if (readCount == 4)
        v = up;
    else if (readCount == 5)
        v = down;
    else if (readCount == 6)
        v = left;
    else if (readCount == 7)
        v = right;

    readCount = (readCount + 1) % 8;

    return v;
}//readReg()


void Joypad::writeReg(int v) {
    v = v & 0x01;
    if (v == 0 && write == 1)
        readCount = 0;

    write = v;
}//writeReg()


bool Joypad::getA() {
	return a;
}

void Joypad::setA(bool v) {
	a = v;
}

bool Joypad::getB() {
	return b;
}

void Joypad::setB(bool v) {
	b = v;
}

bool Joypad::getDown() {
	return down;
}

void Joypad::setDown(bool v) {
	down = v;
}

bool Joypad::getLeft() {
	return left;
}

void Joypad::setLeft(bool v) {
	left = v;
}

bool Joypad::getRight() {
	return right;
}

void Joypad::setRight(bool v) {
	right = v;
}

bool Joypad::getSelect() {
	return select;
}

void Joypad::setSelect(bool v) {
	select = v;
}

bool Joypad::getStart() {
	return start;
}

void Joypad::setStart(bool v) {
	start = v;
}

bool Joypad::getUp() {
	return up;
}

void Joypad::setUp(bool v) {
	up = v;
}
