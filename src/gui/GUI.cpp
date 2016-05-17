/*
 * GUI.cpp
 *
 *  Created on: 21 de ago. de 2015
 *      Author: ibon
 */

#include "GUI.hpp"

GUI::GUI(int argc, char** argv) {
	app = new QApplication(argc, argv);

	mainWindow = new MainWindow();
	mainWindow->show();
	app->exec();
}//GUI()

GUI::~GUI() {
	delete mainWindow;
	delete app;
}//~GUI()




