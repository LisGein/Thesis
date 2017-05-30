#include "datasetView.h"
#include "datasetModel.h"
#include "documentTree/dataset.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>
#include <QInputDialog>


DatasetView::DatasetView(QWidget* parent)
	: QTableView(parent)
	, insertShortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
	, clickedCursor_(-1, -1)
	, model_(nullptr)
{
	QObject::connect(insertShortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));
	QObject::connect(this, SIGNAL(addColumn()), this, SLOT(addedColumn()));
	QObject::connect(this, SIGNAL(addRow()), this, SLOT(addedRow()));
	setEditTriggers(QAbstractItemView::AllEditTriggers);
}

DatasetView::~DatasetView()
{

}

void DatasetView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	clickedCursor_ = event->pos();

	QAction *columnAdd = new QAction(tr("Add column"));
	connect(columnAdd, SIGNAL(triggered(bool)), this, SIGNAL(addColumn()));
	menu.addAction(columnAdd);

	QAction *rowAdd = new QAction(tr("Add row"));
	connect(rowAdd, SIGNAL(triggered(bool)), this, SIGNAL(addRow()));
	menu.addAction(rowAdd);

	QAction *del = new QAction(tr("Delete this column"));
	connect(del, SIGNAL(triggered(bool)), this, SLOT(deleteColumnUnderCursor()));
	menu.addAction(del);

	QAction *rename = new QAction(tr("Rename this column"));
	connect(rename, SIGNAL(triggered(bool)), this, SLOT(renameColumnUnderCursor()));
	menu.addAction(rename);

	menu.exec(event->globalPos());
}

void DatasetView::keyPressEvent(QKeyEvent *event)
{
	if (model_  && event->type() == QEvent::KeyPress && event->key() == Qt::Key_Return)
	{
		const QPair<int, int> lastEditable = model_->lastEditable();
		if (lastEditable.first + 1 == model_->rowCount(QModelIndex()) && lastEditable.second +1 == model_->columnCount(QModelIndex()))
			emit addRow();
	}

}

void DatasetView::setModel(DatasetModel *model)
{
	model_ =model;
	QTableView::setModel(model);
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

void DatasetView::renameColumnUnderCursor()
{
	emit renameColumn(columnAt(clickedCursor_.x()));
}

void DatasetView::addedColumn()
{
	QString text = QInputDialog::getText(this, tr("Column name"), tr("Input column name:"));
	if (text.size())
	{
		model_->beginReset();
		model_->dataset_.addColumn(text.toStdString());
		model_->endReset();
	}
}

void DatasetView::addedRow()
{
	model_->beginReset();
	model_->dataset_.addRow();
	model_->endReset();
}

