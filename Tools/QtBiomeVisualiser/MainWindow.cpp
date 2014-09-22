#include "Globals.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QDirIterator>
#include "inifile/iniFile.h"
#include "ChunkSource.h"
#include "Generating/BioGen.h"
#include "StringCompression.h"
#include "WorldStorage/FastNBT.h"
#include "GeneratorSetup.h"





MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent),
	m_GeneratorSetup(nullptr),
	m_LineSeparator(nullptr)
{
	initMinecraftPath();

	m_BiomeView = new BiomeView();
	m_MainLayout = new QHBoxLayout();
	m_MainLayout->addWidget(m_BiomeView);
	m_MainLayout->setMenuBar(menuBar());
	m_MainLayout->setMargin(0);
	QWidget * central = new QWidget();
	central->setLayout(m_MainLayout);
	setCentralWidget(central);

	createActions();
	createMenus();
}





MainWindow::~MainWindow()
{

}





void MainWindow::newGenerator()
{
	// (Re-)open the generator setup dialog with empty settings:
	openGeneratorSetup("");

	// Set the chunk source:
	m_BiomeView->setChunkSource(std::shared_ptr<BioGenSource>(new BioGenSource(m_GeneratorSetup->getIniFile())));
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
	m_BiomeView->setChunkSource(std::shared_ptr<BioGenSource>(new BioGenSource(m_GeneratorSetup->getIniFile())));
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
	m_BiomeView->setChunkSource(std::shared_ptr<AnvilSource>(new AnvilSource(regionFolder)));
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
	m_BiomeView->setChunkSource(std::shared_ptr<AnvilSource>(new AnvilSource(action->data().toString())));
	m_BiomeView->redraw();
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
	QMenu * file = menuBar()->addMenu(tr("&Map"));
	file->addAction(m_actNewGen);
	file->addAction(m_actOpenGen);
	file->addSeparator();
	QMenu * worlds = file->addMenu(tr("Open existing"));
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
	OutputDebugStringA(name.c_str());
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




