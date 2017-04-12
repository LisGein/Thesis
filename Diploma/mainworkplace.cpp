#include "mainworkplace.h"
#include "table.h"
#include <QTreeView>
#include <QHBoxLayout>


MainWorkPlace::MainWorkPlace(QWidget* parent)
	: QWidget(parent)
	, treeview_(new QTreeView(this))
	, table_(new TableWidget(this))
{
	setLayout(new QHBoxLayout(this));
	layout()->addWidget(treeview_);
	layout()->addWidget(table_);

	table_->setRowCount(10);
	table_->setColumnCount(5);
}

MainWorkPlace::~MainWorkPlace()
{

}
