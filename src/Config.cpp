/*
 * Config.cpp
 *
 *  Created on: 22 de may. de 2016
 *      Author: ibon
 */

#include "Config.hpp"

Config::Config() {
	romFile = "Prueba inicial";
	pause = false;
}

Config::~Config() {

}

void Config::setRomFile(string romFile) {
	this->romFile = romFile;
	updated = true;
}

string Config::getRomFile() {
	return this->romFile;
}

void Config::setPause(bool p) {
	pause = p;
	updated = true;
}

bool Config::getUpdated() {
	return updated;
}

void Config::setUpdated(bool u) {
	updated = u;
}

bool Config::getPause() {
	return pause;
}
