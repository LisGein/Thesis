#include "mainWindow.h"
#include "featureWidget/featureGraphicsScene.h"
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QVBoxLayout>
#include "mainWidget.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainworkplace_(new MainWidget(this))
	, appSettings_(std::make_unique<QSettings>())
{
	initMenu();
	auto size = appSettings_->value("WindowSize", QSize(640, 480));
	resize(size.value<QSize>());
	QMainWindow::setCentralWidget(mainworkplace_);
}

MainWindow::~MainWindow()
{
	appSettings_->setValue("WindowSize", size());
}

void MainWindow::initMenu()
{
	setMenuBar(new QMenuBar(this));
	QMenu* file_menu = menuBar()->addMenu(QObject::tr("File"));

	QAction* new_file  = new QAction(tr("Create model"), this);
	file_menu->addAction(new_file);

	QAction* open_file  = new QAction(tr("Open"), this);
	file_menu->addAction(open_file);

	QAction* save_file  = new QAction(tr("Save"), this);
	file_menu->addAction(save_file);

	QAction* save_as  = new QAction(tr("Save as"), this);
	file_menu->addAction(save_as);

	QAction* exit  = new QAction(tr("Exit"), this);
	file_menu->addAction(exit);
}


