/*
 * MMC1.hpp
 *
 *  Created on: 6 de jul. de 2015
 *      Author: ibon
 */

#ifndef MAPPERS_MMC1_HPP_
#define MAPPERS_MMC1_HPP_

#include "Mapper.hpp"

class ROM;

class MMC1 : public Mapper {
public:
	MMC1(ROM* rom);
	virtual ~MMC1();
};

#endif /* MAPPERS_MMC1_HPP_ */
