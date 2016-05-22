/*
 * Config.hpp
 *
 *  Created on: 22 de may. de 2016
 *      Author: ibon
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

using std::__cxx11::string;


/*
 * Esta clase alamcena y permite modificar los parámetros de configuración
 * y ajustes en tiempo de ejecución
 */
class Config {
public:
	Config();
	virtual ~Config();

	void setRomFile(string romFile);
	string getRomFile();

protected:
	string romFile;
};

#endif /* CONFIG_HPP_ */
