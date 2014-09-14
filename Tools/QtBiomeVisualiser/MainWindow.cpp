#include "Globals.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include "inifile/iniFile.h"
#include "ChunkSource.h"
#include "Generating/BioGen.h"





MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent)
{
	createActions();
	createMenus();

	m_BiomeView = new BiomeView(this);
	setCentralWidget(m_BiomeView);
}





MainWindow::~MainWindow()
{

}





void MainWindow::generate()
{
	QString worldIni = QFileDialog::getOpenFileName(this, tr("Open world.ini"), QString(), tr("world.ini (world.ini)"));
	cIniFile ini;
	if (!ini.ReadFile(worldIni.toStdString()))
	{
		return;
	}
	int seed = ini.GetValueSetI("Seed", "Seed", 0);
	bool unused = false;
	cBiomeGen * biomeGen = cBiomeGen::CreateBiomeGen(ini, seed, unused);
	if (biomeGen == nullptr)
	{
		return;
	}
	m_BiomeView->setChunkSource(std::shared_ptr<BioGenSource>(new BioGenSource(biomeGen)));
	m_BiomeView->redraw();
}





void MainWindow::open()
{
	// TODO
}





void MainWindow::createActions()
{
	m_actGen = new QAction(tr("&Generate..."), this);
	m_actGen->setShortcut(tr("Ctrl+N"));
	m_actGen->setStatusTip(tr("Open a generator INI file and display the generated biomes"));
	connect(m_actGen, SIGNAL(triggered()), this, SLOT(generate()));

	m_actOpen = new QAction(tr("&Open world..."), this);
	m_actOpen->setShortcut(tr("Ctrl+O"));
	m_actOpen->setStatusTip(tr("Open an existing world and display its biomes"));
	connect(m_actOpen, SIGNAL(triggered()), this, SLOT(open()));

	m_actExit = new QAction(tr("E&xit"), this);
	m_actExit->setShortcut(tr("Alt+X"));
	m_actExit->setStatusTip(tr("Exit %1").arg(QApplication::instance()->applicationName()));
	connect(m_actExit, SIGNAL(triggered()), this, SLOT(close()));
}





void MainWindow::createMenus()
{
	QMenu * mFile = menuBar()->addMenu(tr("&World"));
	mFile->addAction(m_actGen);
	mFile->addAction(m_actOpen);
	mFile->addSeparator();
	mFile->addAction(m_actExit);
}




