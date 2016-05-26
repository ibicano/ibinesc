/*
 * Config.cpp
 *
 *  Created on: 22 de may. de 2016
 *      Author: ibon
 */

#include "Config.hpp"

Config::Config() {
	romFile = "Prueba inicial";
	updatedRomFile = false;

	pause = false;
	reset = false;
}

Config::~Config() {

}

void Config::setRomFile(string romFile) {
	this->romFile = romFile;
	updated = true;
	updatedRomFile = true;
}


void Config::setUpdatedRomFile(bool u) {
	updatedRomFile = u;
}


bool Config::getUpdatedRomFile() {
	return updatedRomFile;
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

	if (!u) {
		updatedRomFile = false;
	}
}


bool Config::getPause() {
	return pause;
}


void Config::setReset(bool r) {
	reset = r;
	updated = true;
}


bool Config::getReset() {
	return reset;
}
