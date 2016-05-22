/*
 * Config.cpp
 *
 *  Created on: 22 de may. de 2016
 *      Author: ibon
 */

#include "Config.hpp"

Config::Config() {
	romFile = "Prueba inicial";
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}

void Config::setRomFile(string romFile) {
	this->romFile = romFile;
}

string Config::getRomFile() {
	return this->romFile;
}

