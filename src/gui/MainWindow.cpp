/*
 * MainWindow.cpp
 *
 *  Created on: 17 de may. de 2016
 *      Author: ibon
 */

#include "MainWindow.hpp"
#include "../Config.hpp"

MainWindow::MainWindow(Config* config) {
	this->config = config;
	this->createMenus();
}

MainWindow::~MainWindow() {
	this->destroyMenus();
}

void MainWindow::createMenus() {
	// Crea el menú "File"
	menuFile = this->menuBar()->addMenu("File");
	actionLoadRom = menuFile->addAction("Load Rom...");
	connect(actionLoadRom, SIGNAL(triggered()), this, SLOT(loadFile()));

	menuFile->addSeparator();
	actionQuit = menuFile->addAction("Quit");

	// Crea el menú "Emulator"
	menuEmulator = this->menuBar()->addMenu("Emulator");

	// TODO: hacer que cuando se pulse el botón de pausa se quede marcado como activado con un asterisco o algo
	actionPause = menuEmulator->addAction("Pause");
	connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));

	menuEmulator->addSeparator();
	actionOptions = menuEmulator->addAction("Options...");

	// Crea el menú "Help"
	menuHelp = this->menuBar()->addMenu("Help");
	actionAbout = menuHelp->addAction("About...");
}//createMenus()


//TODO: liberar memoria de los menús
void MainWindow::destroyMenus() {

}//destroyMenus()


// SLOTS

// Abre un menú de diálogo para cargar un fichero
void MainWindow::loadFile() {
	QFileDialog::getOpenFileName(this, "Open ROM file");
}

// Pone el emulador en pause
void MainWindow::pause() {
	config->setPause(!config->getPause());
}


