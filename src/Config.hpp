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

	bool getUpdated();
	void setUpdated(bool u);

	void setUpdatedRomFile(bool u);
	bool getUpdatedRomFile();
	void setRomFile(string romFile);
	string getRomFile();

	void setPause(bool p);
	bool getPause();

	void setReset(bool r);		// Resetea el emulador
	bool getReset();

protected:
	bool updated;		// Indica si la configuración se ha modificado (se pone a true cuando se ejecuta cualquier función que modifique algo)

	string romFile;
	bool updatedRomFile;

	// Estados de ejecución
	bool pause;
	bool reset;
};

#endif /* CONFIG_HPP_ */
