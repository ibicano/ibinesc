/*
 * MainWindow.hpp
 *
 *  Created on: 17 de may. de 2016
 *      Author: ibon
 */

#ifndef GUI_MAINWINDOW_HPP_
#define GUI_MAINWINDOW_HPP_

#include <QtWidgets/QtWidgets>

class Config;

class MainWindow : public QMainWindow {

Q_OBJECT

public:
	MainWindow(Config* config);
	virtual ~MainWindow();

protected:
	// Configuración del emulador
	Config* config;

	// Menús
	QMenu* menuFile;
	QAction* actionLoadRom;
	QAction* actionQuit;

	QMenu* menuHelp;
	QAction* actionAbout;

	QMenu* menuEmulator;
	QAction* actionPause;
	QAction* actionOptions;

	// Crea los menús de la ventana principal
	void createMenus();
	void destroyMenus();


protected slots:
	void loadFile();

};

#endif /* GUI_MAINWINDOW_HPP_ */
