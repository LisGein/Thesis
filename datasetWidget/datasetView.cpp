#include "datasetView.h"
#include "datasetModel.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>


DatasetView::DatasetView(QWidget* parent)
	: QTableView(parent)
	, insertShortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
	, clickedCursor_(-1, -1)
{
	QObject::connect(insertShortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));
	setEditTriggers(QAbstractItemView::AllEditTriggers);
}

DatasetView::~DatasetView()
{

}

void DatasetView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	clickedCursor_ = event->pos();

	QAction *add = new QAction(tr("Add column"));
	connect(add, SIGNAL(triggered(bool)), this, SIGNAL(addColumn()));
	menu.addAction(add);

	QAction *del = new QAction(tr("Delete this column"));
	connect(del, SIGNAL(triggered(bool)), this, SLOT(deleteColumnUnderCursor()));
	menu.addAction(del);

	menu.exec(event->globalPos());
}



void DatasetView::pasteTable()
{
	QString clipboard = QApplication::clipboard()->text().trimmed();
	emit insertedTable(clipboard.toStdString());
}

void DatasetView::deleteColumnUnderCursor()
{
	emit deleteColumn(columnAt(clickedCursor_.x()));
}

