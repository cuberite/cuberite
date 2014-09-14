#pragma once

#include <QMainWindow>
#include "BiomeView.h"





class MainWindow :
	public QMainWindow
{
	Q_OBJECT

	BiomeView * m_BiomeView;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	/** Opens a generator definition and generates the biomes based on that. */
	void generate();

	/** Opens an existing world and displays the loaded biomes. */
	void open();

protected:
	// Actions:
	QAction * m_actGen;
	QAction * m_actOpen;
	QAction * m_actExit;


	/** Creates the actions that the UI supports. */
	void createActions();

	/** Creates the menu bar and connects its events. */
	void createMenus();
};






