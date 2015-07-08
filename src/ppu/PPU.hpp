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

	// Lee y establece el valor del indicador de vblank
	bool getIntVblank();
	void setIntVblank(bool v);
};

#endif /* PPU_PPU_HPP_ */
