#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mixture.h"
#include "chemicalSubstance.h"
#include "reaction.h"
#include "guisubstance.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   setLayout(new QHBoxLayout(this));
   layout()->addWidget(new GuiSubstance(this));
}

MainWindow::~MainWindow()
{
}
