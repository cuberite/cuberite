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





MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent)
{
	initMinecraftPath();

	m_BiomeView = new BiomeView(this);
	setCentralWidget(m_BiomeView);

	createActions();
	createMenus();
}





MainWindow::~MainWindow()
{

}





void MainWindow::generate()
{
	QString worldIni = QFileDialog::getOpenFileName(this, tr("Open world.ini"), QString(), tr("world.ini (world.ini)"));
	if (worldIni.isEmpty())
	{
		return;
	}
	m_BiomeView->setChunkSource(std::shared_ptr<BioGenSource>(new BioGenSource(worldIni)));
	m_BiomeView->redraw();
}





void MainWindow::open()
{
	QString regionFolder = QFileDialog::getExistingDirectory(this, tr("Select the region folder"), QString());
	if (regionFolder.isEmpty())
	{
		return;
	}
	m_BiomeView->setChunkSource(std::shared_ptr<AnvilSource>(new AnvilSource(regionFolder)));
	m_BiomeView->redraw();
}





void MainWindow::openVanillaWorld()
{
	QAction * action = qobject_cast<QAction *>(sender());
	if (action == nullptr)
	{
		return;
	}
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

	m_actGen = new QAction(tr("&Generate..."), this);
	m_actGen->setShortcut(tr("Ctrl+N"));
	m_actGen->setStatusTip(tr("Open a generator INI file and display the generated biomes"));
	connect(m_actGen, SIGNAL(triggered()), this, SLOT(generate()));

	m_actOpen = new QAction(tr("&Open world..."), this);
	m_actOpen->setShortcut(tr("Ctrl+O"));
	m_actOpen->setStatusTip(tr("Open an existing world and display its biomes"));
	connect(m_actOpen, SIGNAL(triggered()), this, SLOT(open()));

	m_actReload = new QAction(tr("&Reload"), this);
	m_actReload->setShortcut(tr("F5"));
	m_actReload->setStatusTip(tr("Open an existing world and display its biomes"));
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
	file->addAction(m_actGen);
	file->addSeparator();
	QMenu * worlds = file->addMenu(tr("Open existing"));
	worlds->addActions(m_WorldActions);
	if (m_WorldActions.empty())
	{
		worlds->setEnabled(false);
	}
	file->addAction(m_actOpen);
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




