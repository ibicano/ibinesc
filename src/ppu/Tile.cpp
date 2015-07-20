/*
 * Tile.cpp
 *
 *  Created on: 18 de jul. de 2015
 *      Author: ibon
 */

#include "Tile.hpp"
#include "PPU.hpp"

Tile::Tile() {
	index = new int*[8];
	rgb = new RGB*[8];

	for (int i = 0; i < 8; i++){
		index[i] = new int[8];
		rgb[i] = new RGB[8];
	}
}


Tile::~Tile() {
	for (int i = 0; i < 8; i++){
		delete[] rgb[i];
		delete[] index[i];
	}

	delete[] rgb;
	delete[] index;
}


int Tile::getIndex(int x, int y) {
	return index[x][y];
}


RGB Tile::getRgb(int x, int y) {
	return rgb[x][y];
}

void Tile::setIndex(int x, int y, int v) {
	index[x][y] = v;
}

void Tile::setRgb(int x, int y, RGB v) {
	rgb[x][y] = v;
}
