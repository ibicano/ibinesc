/*
 * Config.cpp
 *
 *  Created on: 22 de may. de 2016
 *      Author: ibon
 */

#include "Config.hpp"

Config::Config() {
	romFile = "Prueba inicial";
	updated = false;
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


bool Config::getUpdatedRomFile() const {
	return updatedRomFile;
}


string Config::getRomFile() const {
	return this->romFile;
}


void Config::setPause(bool p) {
	pause = p;
	updated = true;
}


bool Config::getUpdated() const {
	return updated;
}


void Config::setUpdated(bool u) {
	updated = u;

	if (!u) {
		updatedRomFile = false;
	}
}


bool Config::getPause() const {
	return pause;
}


void Config::setReset(bool r) {
	reset = r;
	updated = true;
}


bool Config::getReset() const {
	return reset;
}


int Config::getResolution() const {
	return resolution;
}

void Config::setResolution(int r) {
	resolution = r;
	updated = true;
}


bool Config::getFullscreen() {
	return fullscreen;
}


void Config::setFullscreen(bool f) {
	fullscreen = f;
	updated = true;
}
