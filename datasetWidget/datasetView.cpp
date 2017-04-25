#include "datasetView.h"
#include "datasetModel.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>


DatasetView::DatasetView(QWidget* parent)
	: QTableView(parent)
	, insertShortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
{
	QObject::connect(insertShortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));
}

DatasetView::~DatasetView()
{

}

void DatasetView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	//menu.addAction(insert_action_);
	menu.exec(event->globalPos());
}


void DatasetView::pasteTable()
{
	QString clipboard = QApplication::clipboard()->text().trimmed();
	emit insertedTable(clipboard);
}


