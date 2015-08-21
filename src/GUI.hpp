/*
 * GUI.hpp
 *
 *  Created on: 21 de ago. de 2015
 *      Author: ibon
 */

#ifndef GUI_HPP_
#define GUI_HPP_

#include <QtWidgets/QtWidgets>

class GUI {
public:
	GUI(int argc, char** argv);
	virtual ~GUI();

private:
	QApplication* app;			// Aplicación
	QMainWindow* mainWindow;	// Ventana principal

	// Menús
	QMenu* menuFile;
	QAction* actionLoadRom;
	QAction* actionQuit;

	QMenu* menuHelp;
	QAction* actionAbout;

	QMenu* menuEmulator;
	QAction* actionPause;
	QAction* actionOptions;

	void createMainWindow();
	void createMenus();

	void destroyMainWindow();
	void destroyMenus();
};

#endif /* GUI_HPP_ */
