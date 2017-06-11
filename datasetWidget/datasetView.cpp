#include "datasetView.h"
#include "datasetModel.h"
#include "documentTree/dataset.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>
#include <QInputDialog>
#include <QHeaderView>
#include <QDebug>


DatasetView::DatasetView(QWidget* parent)
	: QTableView(parent)
	, insertShortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
	, clickedCursor_(-1, -1)
	, model_(nullptr)
	, columnAdd_( new QAction(QObject::tr("Add column")))
	, rowAdd_( new QAction(QObject::tr("Add row")))
	, deleteCol_( new QAction(QObject::tr("Delete this column")))
	, deleteRow_( new QAction(QObject::tr("Delete this row")))
	, renameCol_( new QAction(QObject::tr("Rename this column")))
{
	QObject::connect(insertShortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));

	setEditTriggers(QAbstractItemView::AllEditTriggers);
	horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(headerContextMenu(const QPoint&)));
}

DatasetView::~DatasetView()
{

}

void DatasetView::contextMenuEvent(QContextMenuEvent* event)
{
	clickedCursor_ = event->globalPos();
	QMenu menu(this);
	generateMenu(menu);

	menu.exec(event->globalPos());
}

void DatasetView::keyPressEvent(QKeyEvent *event)
{
	if (model_  && event->type() == QEvent::KeyPress && event->key() == Qt::Key_Return)
	{/*
		const QPair<int, int> lastEditable = model_->lastEditable();
		if (lastEditable.first + 1 == model_->rowCount(QModelIndex()) && lastEditable.second +1 == model_->columnCount(QModelIndex()))
			emit addRow();*/
	}

}

void DatasetView::setModel(DatasetModel *model)
{
	model_ = model;
	QObject::connect(columnAdd_, SIGNAL(triggered(bool)), this, SLOT(addColumn()));
	QObject::connect(rowAdd_, SIGNAL(triggered(bool)), model_, SLOT(addRow()));
	QObject::connect(deleteCol_, SIGNAL(triggered(bool)), this, SLOT(deleteColumnUnderCursor()));
	QObject::connect(deleteRow_, SIGNAL(triggered(bool)), this, SLOT(deleteRowUnderCursor()));
	QObject::connect(renameCol_, SIGNAL(triggered(bool)), this, SLOT(renameColumnUnderCursor()));
	QTableView::setModel(model);
}



void DatasetView::pasteTable()
{
	QString clipboard = QApplication::clipboard()->text().trimmed();
	emit insertedTable(clipboard.toStdString());
}

void DatasetView::addColumn()
{
	QString text = QInputDialog::getText(this, QObject::tr("Column name"), QObject::tr("Input column name:"));
	if (text.size())
	{
		model_->addColumn(text);
	}
}

void DatasetView::deleteColumnUnderCursor()
{
	QPoint point = viewport()->mapFromGlobal(clickedCursor_);
	model_->deleteColumn(columnAt(point.x()));
}

void DatasetView::deleteRowUnderCursor()
{
	QPoint point = viewport()->mapFromGlobal(clickedCursor_);
	model_->deleteRow(rowAt(point.y()));
}

void DatasetView::renameColumnUnderCursor()
{
	QString text = QInputDialog::getText(this, QObject::tr("Column name"), QObject::tr("Input column name:"));

	QPoint point = viewport()->mapFromGlobal(clickedCursor_);
	model_->renameColumn(columnAt(point.x()), text);
}

void DatasetView::headerContextMenu(const QPoint& point)
{
	clickedCursor_ = viewport()->mapToGlobal(point);

	QMenu menu(this);
	generateMenu(menu);

	menu.exec(viewport()->mapToGlobal(point));
}

void DatasetView::generateMenu(QMenu &menu)
{
	menu.addAction(columnAdd_);
	menu.addAction(rowAdd_);

	QPoint point = viewport()->mapFromGlobal(clickedCursor_);
	if (columnAt(point.x()) != -1 && rowAt(point.y()) != -1)
	{
		menu.addAction(deleteCol_);
		menu.addAction(deleteRow_);
		menu.addAction(renameCol_);
	}
}



