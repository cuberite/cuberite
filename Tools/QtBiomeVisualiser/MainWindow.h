#pragma once

#include <memory>
#include <QList>
#include <QMainWindow>
#include <QHBoxLayout>
#include "BiomeView.h"





// fwd:
class GeneratorSetup;





class MainWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

private slots:
	/** Creates a generator definition from scratch, lets user modify generator params in realtime. */
	void newGenerator();

	/** Opens a generator definition and generates the biomes based on that. */
	void openGenerator();

	/** Opens an existing world and displays the loaded biomes. */
	void openWorld();

	/** Opens a vanilla world that is specified by the calling action. */
	void openVanillaWorld();

protected:
	// Actions:
	QAction * m_actNewGen;
	QAction * m_actOpenGen;
	QAction * m_actOpenWorld;
	QAction * m_actReload;
	QAction * m_actExit;

	/** List of actions that open the specific vanilla world. */
	QList<QAction *> m_WorldActions;

	/** Path to the vanilla folder. */
	QString m_MinecraftPath;

	/** The pane for setting up the generator, available when visualising a generator. */
	GeneratorSetup * m_GeneratorSetup;

	/** The main biome display widget. */
	BiomeView * m_BiomeView;

	/** The layout for the window. */
	QHBoxLayout * m_MainLayout;

	/** The separator line between biome view and generator setup. */
	QWidget * m_LineSeparator;


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

	/** Opens the generator setup pane, if not already open, and loads the specified INI file to it. */
	void openGeneratorSetup(const AString & a_IniFileName);

	/** Closes and destroys the generator setup pane, if there is one. */
	void closeGeneratorSetup();
};






