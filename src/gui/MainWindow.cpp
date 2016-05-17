/*
 * MainWindow.cpp
 *
 *  Created on: 17 de may. de 2016
 *      Author: ibon
 */

#include "MainWindow.hpp"

MainWindow::MainWindow() {
	this->createMenus();
}

MainWindow::~MainWindow() {
	this->destroyMenus();
}

void MainWindow::createMenus() {
	// Crea el menú "File"
	menuFile = this->menuBar()->addMenu("File");
	actionLoadRom = menuFile->addAction("Load Rom...");
	QObject::connect(actionLoadRom, SIGNAL(triggered()), this, SLOT(loadFile()));

	menuFile->addSeparator();
	actionQuit = menuFile->addAction("Quit");

	// Crea el menú "Emulator"
	menuEmulator = this->menuBar()->addMenu("Emulator");
	actionPause = menuEmulator->addAction("Pause");
	menuEmulator->addSeparator();
	actionOptions = menuEmulator->addAction("Options...");

	// Crea el menú "Help"
	menuHelp = this->menuBar()->addMenu("Help");
	actionAbout = menuHelp->addAction("About...");
}//createMenus()


//TODO: liberar memoria de los menús
void MainWindow::destroyMenus() {

}//destroyMenus()


void MainWindow::loadFile() {
	QFileDialog::getOpenFileName();
}


