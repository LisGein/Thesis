#include <QApplication>
#include <QTabWidget>
#include "fuel.h"
#include "calccombustion.h"
#include "clacflow.h"


int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   QTabWidget* tabWidget_ = new QTabWidget;
   tabWidget_->setTabPosition(QTabWidget::West);
   Fuel* f = new Fuel();
   tabWidget_->addTab(f, QString("Fuel"));
   CalcCombustion* calcCombustion = new CalcCombustion();
   tabWidget_->addTab(calcCombustion, QString("Calc combustion"));
   ClacFlow* clacFlow = new ClacFlow();
   tabWidget_->addTab(clacFlow, QString("Calc flow"));

   tabWidget_->show();

   return a.exec();
}
