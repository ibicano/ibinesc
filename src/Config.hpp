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

	bool getUpdated() const;
	void setUpdated(bool u);

	void setUpdatedRomFile(bool u);
	bool getUpdatedRomFile() const;
	void setRomFile(string romFile);
	string getRomFile() const;

	void setPause(bool p);
	bool getPause() const;

	void setReset(bool r);		// Resetea el emulador
	bool getReset() const;

	int getResolution() const;
	void setResolution(int r);

	bool getFullscreen();
	void setFullscreen(bool f);

protected:
	bool updated;				// Indica si la configuración se ha modificado (se pone a true cuando se ejecuta cualquier función que modifique algo)

	string romFile;				// Nombre del fichero de la ROM
	bool updatedRomFile;		// Indica si ha cambiado el fichero de ROM para cargarlo de nuevo

	int resolution = 1;			// Factor de multiplicación de la resolución de vídeo

	bool fullscreen = false; 	// Indica si el emulador se ejecuta en pantalla completa

	// Estados de ejecución
	bool pause;
	bool reset;
};

#endif /* CONFIG_HPP_ */
