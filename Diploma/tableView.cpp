#include "tableView.h"
#include "tableModel.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>


TableView::TableView(QWidget* parent)
	: QTableView(parent)
	, model_(new TableModel(this))
	, insertShortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
{
	setModel(model_);
	QObject::connect(insertShortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));
}

TableView::~TableView()
{

}

void TableView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	//menu.addAction(insert_action_);
	menu.exec(event->globalPos());
}

QVector<double> TableView::dataFromLines(const QString& first, const QString& second) const
{
	return model_->dataFromLines(QPair<QString, QString>(first, second));
}

void TableView::pasteTable()
{
	QString clipboard = QApplication::clipboard()->text().trimmed();
	QStringList params = parseTable(clipboard.trimmed());
	emit insertedTable(params);
}

const QStringList TableView::parseTable(const QString& str)
{
	model_->beginReset();

	QStringList params;
	QStringList rows = str.split("\n");


	int row = 0;
	for(auto it_r = rows.begin(); it_r != rows.end(); ++ it_r)
	{
		QStringList columns = it_r->split("\t");

		if (it_r == rows.begin())
		{
			model_->setHorizontalHeaderLabels(columns);
			params = columns;
		}
		else
		{
			int column = 0;
			for(auto it_c = columns.begin(); it_c != columns.end(); ++ it_c)
			{
				model_->setData(row, column, QVariant(*it_c));
				++column;
			}
			++row;
		}
	}
	//calcParams(i_data,i_res, column, row);

	model_->endReset();
	update();
	return params;
}
