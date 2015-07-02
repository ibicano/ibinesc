/*
 * ROM.h
 *
 *  Created on: 2 de jul. de 2015
 *      Author: ibon
 */

#ifndef ROM_HPP_
#define ROM_HPP_
#include <iosfwd>

using std::string;

class ROM {
public:
	ROM();
	virtual ~ROM();

	// Carga la ROM de un fichero
	int load_file(string filename);

	/*
	 * Devuelve el modo de mirroring indicado en la ROM:
	 * 0: horizontal
	 * 1: vertical
	 * 2: single
	 * 3: 4-screen
	 */
	int get_mirroring();

	// Devuelve el código del Mapper de la ROM
	int get_mapper_code();

	// Devuelve el número de bancos de programa de 16KB presentes en la ROM
	int get_prg_count();

	// Devuelve el número de bancos de patrones de 8KB presentes en la ROM
	int get_chr_count();

	//TODO: Acabar la especificación de las funciones

};

#endif /* ROM_HPP_ */
