#include "mainWidget.h"
#include <QVBoxLayout>
#include "tableWidget.h"
#include "regressionModel.h"
#include <QGraphicsView>
#include <QLineEdit>


MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, scene_(new GraphicsScene(this))
{
	setLayout(new QVBoxLayout(this));

	TableWidget *table = new TableWidget(this);
	table->setColumnCount(10);
	table->setRowCount(10);
	layout()->addWidget(table);

	QLineEdit* text = new QLineEdit(this);
	layout()->addWidget(text);



	QGraphicsView *gview = new QGraphicsView(this);
	gview->setScene(scene_);
	layout()->addWidget(gview);

	QObject::connect(table, SIGNAL(insertedTable(QStringList)), this, SLOT(convertInsertedTable(QStringList)));

}

MainWidget::~MainWidget()
{

}

void MainWidget::convertInsertedTable(QStringList list)
{
	list.push_front("1");
	list.pop_back();
	scene_->updateTable(list);
}

