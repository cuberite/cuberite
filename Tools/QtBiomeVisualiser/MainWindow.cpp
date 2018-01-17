#include "Globals.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QDirIterator>
#include <QStatusBar>
#include "ChunkSource.h"
#include "src/IniFile.h"
#include "src/Generating/BioGen.h"
#include "src/StringCompression.h"
#include "src/WorldStorage/FastNBT.h"
#include "GeneratorSetup.h"
#include "RegionLoader.h"





const double MainWindow::m_ViewZooms[] =
{
	0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 24,
};





MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent),
	m_GeneratorSetup(nullptr),
	m_LineSeparator(nullptr),
	m_CurrentZoomLevel(2)
{
	initMinecraftPath();

	m_BiomeView = new BiomeView();
	connect(m_BiomeView, SIGNAL(increaseZoom()), this, SLOT(increaseZoom()));
	connect(m_BiomeView, SIGNAL(decreaseZoom()), this, SLOT(decreaseZoom()));
	connect(m_BiomeView, SIGNAL(wheelUp()),      this, SLOT(increaseZoom()));
	connect(m_BiomeView, SIGNAL(wheelDown()),    this, SLOT(decreaseZoom()));
	m_BiomeView->setZoomLevel(m_ViewZooms[m_CurrentZoomLevel]);

	m_StatusBar = new QStatusBar();
	this->setStatusBar(m_StatusBar);
	m_StatusBlockX = new QLabel(tr("X"));
	m_StatusBlockZ = new QLabel(tr("Z"));
	m_StatusBiome = new QLabel(tr("B"));
	m_StatusBar->addPermanentWidget(m_StatusBlockX);
	m_StatusBar->addPermanentWidget(m_StatusBlockZ);
	m_StatusBar->addPermanentWidget(m_StatusBiome);

	m_MainLayout = new QHBoxLayout();
	m_MainLayout->addWidget(m_BiomeView, 1);
	m_MainLayout->setMenuBar(menuBar());
	m_MainLayout->setMargin(0);
	QWidget * central = new QWidget();
	central->setLayout(m_MainLayout);
	setCentralWidget(central);

	createActions();
	createMenus();

	connect(m_BiomeView, SIGNAL(hoverChanged(int, int, int)), this, SLOT(hoverChanged(int, int, int)));
}





MainWindow::~MainWindow()
{
	RegionLoader::shutdown();
}





void MainWindow::newGenerator()
{
	// (Re-)open the generator setup dialog with empty settings:
	openGeneratorSetup("");

	// Set the chunk source:
	cIniFilePtr iniFile = m_GeneratorSetup->getIniFile();
	m_BiomeView->setChunkSource(std::make_shared<BioGenSource>(iniFile));
	m_BiomeView->redraw();
}





void MainWindow::openGenerator()
{
	// Let the user specify the world.ini file:
	QString worldIni = QFileDialog::getOpenFileName(this, tr("Open world.ini"), QString(), tr("world.ini (world.ini)"));
	if (worldIni.isEmpty())
	{
		return;
	}

	// (Re-)open the generator setup dialog:
	openGeneratorSetup(worldIni.toStdString());

	// Set the chunk source:
	m_BiomeView->setChunkSource(std::make_shared<BioGenSource>(m_GeneratorSetup->getIniFile()));
	m_BiomeView->redraw();
}





void MainWindow::openWorld()
{
	// Let the user specify the world:
	QString regionFolder = QFileDialog::getExistingDirectory(this, tr("Select the region folder"), QString());
	if (regionFolder.isEmpty())
	{
		return;
	}

	// Remove the generator setup dialog, if open:
	closeGeneratorSetup();

	// Set the chunk source:
	m_BiomeView->setChunkSource(std::make_shared<AnvilSource>(regionFolder));
	m_BiomeView->redraw();
}





void MainWindow::openVanillaWorld()
{
	// The world is stored in the sender action's data, retrieve it:
	QAction * action = qobject_cast<QAction *>(sender());
	if (action == nullptr)
	{
		return;
	}

	// Remove the generator setup dialog, if open:
	closeGeneratorSetup();

	// Set the chunk source:
	m_BiomeView->setChunkSource(std::make_shared<AnvilSource>(action->data().toString()));
	m_BiomeView->redraw();
}





void MainWindow::centerView()
{
	m_BiomeView->setPosition(0, 0);
}





void MainWindow::setViewZoom()
{
	// The zoom level is stored in the sender action's data, retrieve it:
	QAction * action = qobject_cast<QAction *>(sender());
	if (action == nullptr)
	{
		return;
	}
	m_CurrentZoomLevel = action->data().toInt();
	double newZoom = m_ViewZooms[m_CurrentZoomLevel];
	m_BiomeView->setZoomLevel(newZoom);
	action->setChecked(true);
}





void MainWindow::increaseZoom()
{
	// If already at max zoom, bail out:
	if (m_CurrentZoomLevel >= ARRAYCOUNT(m_ViewZooms) - 1)
	{
		return;
	}

	// Increase the zoom level:
	m_CurrentZoomLevel += 1;
	m_actViewZoom[m_CurrentZoomLevel]->setChecked(true);
	m_BiomeView->setZoomLevel(m_ViewZooms[m_CurrentZoomLevel]);
}





void MainWindow::decreaseZoom()
{
	// If already at min zoom, bail out:
	if (m_CurrentZoomLevel == 0)
	{
		return;
	}

	// Decrease the zoom level:
	m_CurrentZoomLevel -= 1;
	m_actViewZoom[m_CurrentZoomLevel]->setChecked(true);
	m_BiomeView->setZoomLevel(m_ViewZooms[m_CurrentZoomLevel]);
}





void MainWindow::hoverChanged(int a_BlockX, int a_BlockZ, int a_Biome)
{
	m_StatusBlockX->setText(tr("X: %1").arg(a_BlockX));
	m_StatusBlockZ->setText(tr("Z: %1").arg(a_BlockZ));
	m_StatusBiome->setText (tr("B: %1 (%2)").arg(BiomeToString(a_Biome).c_str()).arg(a_Biome));
}





void MainWindow::initMinecraftPath()
{
	#ifdef Q_OS_MAC
		m_MinecraftPath = QDir::homePath() + QDir::toNativeSeparators("/Library/Application Support/minecraft");
	#elif defined Q_OS_WIN32
		QSettings ini(QSettings::IniFormat, QSettings::UserScope, ".minecraft", "minecraft1");
		m_MinecraftPath = QFileInfo(ini.fileName()).absolutePath();
	#else
		m_MinecraftPath = QDir::homePath() + QDir::toNativeSeparators("/.minecraft");
	#endif
}





void MainWindow::createActions()
{
	// Map menu:
	createWorldActions();

	m_actNewGen = new QAction(tr("&New generator"), this);
	m_actNewGen->setShortcut(tr("Ctrl+N"));
	m_actNewGen->setStatusTip(tr("Open a generator INI file and display the generated biomes"));
	connect(m_actNewGen, SIGNAL(triggered()), this, SLOT(newGenerator()));

	m_actOpenGen = new QAction(tr("&Open generator..."), this);
	m_actOpenGen->setShortcut(tr("Ctrl+G"));
	m_actOpenGen->setStatusTip(tr("Open a generator INI file and display the generated biomes"));
	connect(m_actOpenGen, SIGNAL(triggered()), this, SLOT(openGenerator()));

	m_actOpenWorld = new QAction(tr("&Open world..."), this);
	m_actOpenWorld->setShortcut(tr("Ctrl+O"));
	m_actOpenWorld->setStatusTip(tr("Open an existing world and display its biomes"));
	connect(m_actOpenWorld, SIGNAL(triggered()), this, SLOT(openWorld()));

	m_actReload = new QAction(tr("&Reload"), this);
	m_actReload->setShortcut(tr("F5"));
	m_actReload->setStatusTip(tr("Clear the view cache and force a reload of all the data"));
	connect(m_actReload, SIGNAL(triggered()), m_BiomeView, SLOT(reload()));

	m_actExit = new QAction(tr("E&xit"), this);
	m_actExit->setShortcut(tr("Alt+X"));
	m_actExit->setStatusTip(tr("Exit %1").arg(QApplication::instance()->applicationName()));
	connect(m_actExit, SIGNAL(triggered()), this, SLOT(close()));

	// View menu:
	m_actViewCenter = new QAction(tr("&Reset to center"), this);
	m_actViewCenter->setStatusTip(tr("Scrolls the view back to the map center"));
	connect(m_actViewCenter, SIGNAL(triggered()), this, SLOT(centerView()));

	QActionGroup * zoomGroup = new QActionGroup(this);
	for (int i = 0; i < ARRAYCOUNT(m_ViewZooms); i++)
	{
		m_actViewZoom[i] = new QAction(tr("&Zoom %1%").arg(std::floor(m_ViewZooms[i] * 100)), this);
		m_actViewZoom[i]->setCheckable(true);
		m_actViewZoom[i]->setData(QVariant(i));
		zoomGroup->addAction(m_actViewZoom[i]);
		connect(m_actViewZoom[i], SIGNAL(triggered()), this, SLOT(setViewZoom()));
	}
	m_actViewZoom[m_CurrentZoomLevel]->setChecked(true);
}





void MainWindow::createWorldActions()
{
	QDir mc(m_MinecraftPath);
	if (!mc.cd("saves"))
	{
		return;
	}

	QDirIterator it(mc);
	int key = 1;
	while (it.hasNext())
	{
		it.next();
		if (!it.fileInfo().isDir())
		{
			continue;
		}
		QString name = getWorldName(it.filePath().toStdString());
		if (name.isEmpty())
		{
			continue;
		}
		QAction * w = new QAction(this);
		w->setText(name);
		w->setData(it.filePath() + "/region");
		if (key < 10)
		{
			w->setShortcut("Ctrl+" + QString::number(key));
			key++;
		}
		connect(w, SIGNAL(triggered()), this, SLOT(openVanillaWorld()));
		m_WorldActions.append(w);
	}
}





void MainWindow::createMenus()
{
	// Map menu:
	QMenu * file = menuBar()->addMenu(tr("&Map"));
	file->addAction(m_actNewGen);
	file->addAction(m_actOpenGen);
	file->addSeparator();
	QMenu * worlds = file->addMenu(tr("Open &existing"));
	worlds->addActions(m_WorldActions);
	if (m_WorldActions.empty())
	{
		worlds->setEnabled(false);
	}
	file->addAction(m_actOpenWorld);
	file->addSeparator();
	file->addAction(m_actReload);
	file->addSeparator();
	file->addAction(m_actExit);

	// View menu:
	QMenu * view = menuBar()->addMenu(tr("&View"));
	view->addAction(m_actViewCenter);
	view->addSeparator();
	for (size_t i = 0; i < ARRAYCOUNT(m_actViewZoom); i++)
	{
		view->addAction(m_actViewZoom[i]);
	}
}





QString MainWindow::getWorldName(const AString & a_Path)
{
	AString levelData = cFile::ReadWholeFile(a_Path + "/level.dat");
	if (levelData.empty())
	{
		// No such file / no data
		return QString();
	}

	AString uncompressed;
	if (UncompressStringGZIP(levelData.data(), levelData.size(), uncompressed) != Z_OK)
	{
		return QString();
	}
	cParsedNBT nbt(uncompressed.data(), uncompressed.size());
	if (!nbt.IsValid())
	{
		return QString();
	}
	AString name = nbt.GetName(1);
	int levelNameTag = nbt.FindTagByPath(nbt.GetRoot(), "Data\\LevelName");
	if ((levelNameTag <= 0) || (nbt.GetType(levelNameTag) != TAG_String))
	{
		return QString();
	}
	return QString::fromStdString(nbt.GetString(levelNameTag));
}





void MainWindow::openGeneratorSetup(const AString & a_IniFileName)
{
	// Close any previous editor:
	closeGeneratorSetup();

	// Open up a new editor:
	m_GeneratorSetup = new GeneratorSetup(a_IniFileName);
	m_LineSeparator = new QWidget();
	m_LineSeparator->setFixedWidth(2);
	m_LineSeparator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_LineSeparator->setStyleSheet(QString("background-color: #c0c0c0;"));
	m_MainLayout->addWidget(m_LineSeparator);
	m_MainLayout->addWidget(m_GeneratorSetup);

	// Connect the signals from the setup pane:
	connect(m_GeneratorSetup, SIGNAL(generatorUpdated()), m_BiomeView, SLOT(reload()));
}





void MainWindow::closeGeneratorSetup()
{
	delete m_MainLayout->takeAt(2);
	delete m_MainLayout->takeAt(1);
	delete m_GeneratorSetup;
	delete m_LineSeparator;
	m_GeneratorSetup = nullptr;
	m_LineSeparator = nullptr;
}




