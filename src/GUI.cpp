/*
 * GUI.cpp
 *
 *  Created on: 21 de ago. de 2015
 *      Author: ibon
 */

#include "GUI.hpp"

GUI::GUI(int argc, char** argv) {
	app = new QApplication(argc, argv);
	mainWindow = new QMainWindow();
	createMainWindow();
	createMenus();

	mainWindow->show();
	app->exec();
}//GUI()

GUI::~GUI() {
	destroyMenus();
	destroyMainWindow();

	delete app;
}//~GUI()


void GUI::createMainWindow() {
	mainWindow = new QMainWindow();
}//createMainWindow()


void GUI::createMenus() {
	// Crea el menú "File"
	menuFile = mainWindow->menuBar()->addMenu("File");
	actionLoadRom = menuFile->addAction("Load Rom...");
	menuFile->addSeparator();
	actionQuit = menuFile->addAction("Quit");

	// Crea el menú "Emulator"
	menuEmulator = mainWindow->menuBar()->addMenu("Emulator");
	actionPause = menuEmulator->addAction("Pause");
	menuEmulator->addSeparator();
	actionOptions = menuEmulator->addAction("Options...");

	// Crea el menú "Help"
	menuHelp = mainWindow->menuBar()->addMenu("Help");
	actionAbout = menuHelp->addAction("About...");
}//createMenus()


void GUI::destroyMainWindow() {
	delete mainWindow;
}//destroyMainWindow()


//TODO: liberar memoria de los menús
void GUI::destroyMenus() {

}//destroyMenus()

