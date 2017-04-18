#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regressionModel.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include "table.h"
#include <QGraphicsView>
#include <QVector>


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainworkplace_(new QTabWidget(this))
{
	setLayout(new QVBoxLayout(this));
	initMenu();
	QMainWindow::setCentralWidget(mainworkplace_);

	TableWidget *table = new TableWidget(this);
	mainworkplace_->addTab(table, QObject::tr("Matrix"));
	QGraphicsView *gview = new QGraphicsView(this);
	GraphicsScene* scene = new GraphicsScene(this);
	gview->setScene(scene);
	mainworkplace_->addTab(gview, QObject::tr("Model"));
	scene->updateTable({"q", "w", "e"});

}

MainWindow::~MainWindow()
{

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
