/*
 * GUI.hpp
 *
 *  Created on: 21 de ago. de 2015
 *      Author: ibon
 */

#ifndef GUI_HPP_
#define GUI_HPP_

#include "MainWindow.hpp"

class GUI {

public:
	GUI(int argc, char** argv, Config* config);
	virtual ~GUI();

protected:
	Config* config;				// Configuración y ajustes del emulador
	QApplication* app;			// Aplicación
	MainWindow* mainWindow;

};

#endif /* GUI_HPP_ */
