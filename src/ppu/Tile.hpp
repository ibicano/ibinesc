/*
 * Tile.hpp
 *
 *  Created on: 18 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_TILE_HPP_
#define PPU_TILE_HPP_

struct RGB;

class Tile {
public:
	Tile();
	virtual ~Tile();

	int getIndex(int x, int y);
	RGB getRgb(int x, int y);

	void setIndex(int x, int y, int v);
	void setRgb(int x, int y, RGB v);

private:
	int** index;
	RGB** rgb;
};

#endif /* PPU_TILE_HPP_ */
