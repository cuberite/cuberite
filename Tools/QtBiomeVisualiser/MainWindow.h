#pragma once

#include <QList>
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

	/** Opens a vanilla world that is specified by the calling action. */
	void openVanillaWorld();

protected:
	// Actions:
	QAction * m_actGen;
	QAction * m_actOpen;
	QAction * m_actReload;
	QAction * m_actExit;

	/** List of actions that open the specific vanilla world. */
	QList<QAction *> m_WorldActions;

	/** Path to the vanilla folder. */
	QString m_MinecraftPath;


	/** Initializes the m_MinecraftPath based on the proper MC path */
	void initMinecraftPath();

	/** Creates the actions that the UI supports. */
	void createActions();

	/** Creates the actions that open a specific vanilla world. Iterates over the minecraft saves folder. */
	void createWorldActions();

	/** Creates the menu bar and connects its events. */
	void createMenus();

	/** Returns the name of the vanilla world in the specified path.
	Reads the level.dat file for the name. Returns an empty string on failure. */
	QString getWorldName(const AString & a_Path);
};






