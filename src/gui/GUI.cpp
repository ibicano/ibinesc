/*
 * GUI.cpp
 *
 *  Created on: 21 de ago. de 2015
 *      Author: ibon
 */

#include "GUI.hpp"

GUI::GUI(int argc, char** argv, Config* config) {
	this->config = config;

	app = new QApplication(argc, argv);

	mainWindow = new MainWindow(config);
	mainWindow->show();
	app->exec();
}//GUI()

GUI::~GUI() {
	delete mainWindow;
	delete app;
}//~GUI()




