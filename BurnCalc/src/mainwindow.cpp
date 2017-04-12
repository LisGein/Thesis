#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mixture.h"
#include "chemicalSubstance.h"
#include "reaction.h"
#include "gui/guisubstance.h"
#include "gui/guiempericalformula.h"
#include <QHBoxLayout>
#include <QTextEdit>

CMainWindow::CMainWindow(QWidget *parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
   , substances(new GuiEmpericalFormula())
{
   ui->setupUi(this);

   setLayout(new QVBoxLayout(this));
   layout()->addWidget(substances);

}

CMainWindow::~CMainWindow()
{
}
