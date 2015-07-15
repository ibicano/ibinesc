/*
 * MMC3.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_MMC3_HPP_
#define MAPPERS_MMC3_HPP_

#include "Mapper.hpp"

class ROM;

class MMC3 : public Mapper {
public:
	MMC3(ROM* rom);
	virtual ~MMC3();
};

#endif /* MAPPERS_MMC3_HPP_ */
