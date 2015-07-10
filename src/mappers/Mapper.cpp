/*
 * Mapper.cpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#include "Mapper.hpp"


Mapper::Mapper(ROM* rom) {
	this->rom = rom;
}


Mapper::~Mapper() {

}

int Mapper::getPrgCount() {
	return rom->getPrgCount();
}

int Mapper::getChrCount() {
	return rom->getChrCount();
}
