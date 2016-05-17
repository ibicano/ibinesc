/*
 * MainWindow.hpp
 *
 *  Created on: 17 de may. de 2016
 *      Author: ibon
 */

#ifndef GUI_MAINWINDOW_HPP_
#define GUI_MAINWINDOW_HPP_

#include <QtWidgets/QtWidgets>

class MainWindow : public QMainWindow {

// TODO: investigar para integrar las herramientas de QT en el proyecto
// en especial es necesario ejecutar la herramienta "moc" para que expanda
// las macros propias de QT
Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

protected:
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


public slots:
	void loadFile();

};

#endif /* GUI_MAINWINDOW_HPP_ */
