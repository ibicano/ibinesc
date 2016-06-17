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
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

	// Crea el menú "Emulator"
	menuEmulator = this->menuBar()->addMenu("Emulator");

	actionPause = menuEmulator->addAction("Pause");
	actionPause->setCheckable(true);
	connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));

	actionReset = menuEmulator->addAction("Reset");
	connect(actionReset, SIGNAL(triggered()), this, SLOT(reset()));

	// Submenú con opciones de vídeo
	QSignalMapper* signalMapper = new QSignalMapper(this);

	subVideo = menuEmulator->addMenu("Video");
	actionRes1x = subVideo->addAction("1x");
	connect(actionRes1x, SIGNAL(triggered()), signalMapper, SLOT(map()));
	actionRes2x = subVideo->addAction("2x");
	connect(actionRes2x, SIGNAL(triggered()), signalMapper, SLOT(map()));
	actionRes3x = subVideo->addAction("3x");
	connect(actionRes3x, SIGNAL(triggered()), signalMapper, SLOT(map()));
	actionRes4x = subVideo->addAction("4x");
	connect(actionRes4x, SIGNAL(triggered()), signalMapper, SLOT(map()));

	signalMapper->setMapping(actionRes1x, 1);
	signalMapper->setMapping(actionRes2x, 2);
	signalMapper->setMapping(actionRes3x, 3);
	signalMapper->setMapping(actionRes4x, 4);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(resolution(int)));

	subVideo->addSeparator();

	actionFullscreen = subVideo->addAction("Fullscreen");
	connect(actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));

	menuEmulator->addSeparator();
	actionOptions = menuEmulator->addAction("Options...");

	// Crea el menú "Help"
	menuHelp = this->menuBar()->addMenu("Help");
	actionAbout = menuHelp->addAction("About...");
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}//createMenus()


// Libera memoria de los menús
void MainWindow::destroyMenus() {
	delete actionAbout;
	delete menuHelp;

	delete actionOptions;

	delete actionFullscreen;

	delete actionRes4x;
	delete actionRes3x;
	delete actionRes2x;
	delete actionRes1x;
	delete subVideo;

	delete actionReset;
	delete actionPause;
	delete menuEmulator;

	delete actionQuit;
	delete actionLoadRom;
	delete menuFile;
}//destroyMenus()


// SLOTS

// Abre un menú de diálogo para cargar un fichero
void MainWindow::loadFile() {
	config->setRomFile(qPrintable(QFileDialog::getOpenFileName(this, "Open ROM file")));
}

// Pone el emulador en pause
void MainWindow::pause() {
	config->setPause(!config->getPause());
}

// Resetea el emulador
void MainWindow::reset() {
	config->setReset(true);
}

void MainWindow::about() {
	QMessageBox::about(this, "About...", "IBINES v0.1 beta");
}

void MainWindow::quit() {
	exit(0);
}

void MainWindow::resolution(int r) {
	config->setResolution(r);
}

void MainWindow::fullscreen() {
	config->setFullscreen(true);
}


//EVENTS

// Reimplemento la función closeEvent para que cierre la aplicación cuando secierra la ventana QT
void MainWindow::closeEvent(QCloseEvent* event) {
	exit(0);
}


