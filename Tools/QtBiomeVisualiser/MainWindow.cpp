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

	m_actReload = new QAction(tr("&Reload"), this);
	m_actReload->setShortcut(tr("F5"));
	m_actReload->setStatusTip(tr("Open an existing world and display its biomes"));
	connect(m_actReload, SIGNAL(triggered()), m_BiomeView, SLOT(reload()));

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
	mFile->addAction(m_actReload);
	mFile->addSeparator();
	mFile->addAction(m_actExit);
}




