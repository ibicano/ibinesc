/*
 * PPU.hpp
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef PPU_PPU_HPP_
#define PPU_PPU_HPP_

class PPU {
public:
	PPU();
	virtual ~PPU();

	bool get_int_vblank();
};

#endif /* PPU_PPU_HPP_ */
