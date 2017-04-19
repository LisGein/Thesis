#include "tableWidget.h"

#include "regression.h"

#include <QDebug>
#include <QKeyEvent>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QClipboard>



TableWidget::TableWidget(QWidget* parent)
	: QTableWidget(parent)
	, insert_shortcut_(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this))
{
	QObject::connect(insert_shortcut_, SIGNAL(activated()), this, SLOT(pasteTable()));
}

TableWidget::~TableWidget()
{

}

void TableWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	//menu.addAction(insert_action_);
	menu.exec(event->globalPos());
}

void TableWidget::pasteTable()
{
	QString clipboard = QApplication::clipboard()->text().trimmed();
	qDebug() << clipboard;
	QStringList params = parseTable(clipboard.trimmed());

	emit insertedTable(params);
}

const QStringList TableWidget::parseTable(const QString &str)
{
	QStringList params;

	QStringList rows = str.split("\n");

	int row = 0;
	int column = 0;

	std::vector<double> i_data;
	std::vector<double> i_res;
	for(auto it_r = rows.begin(); it_r != rows.end(); ++ it_r)
	{
		QStringList columns = it_r->split("\t");

		if (it_r == rows.begin())
		{
			setHorizontalHeaderLabels(columns);
			params = columns;
		}
		else
		{
			for(auto it_c = columns.begin(); it_c != columns.end(); ++ it_c)
			{
				if (it_c == columns.end() - 1)
				{
					i_res.push_back(it_c->toDouble());
				}
				else
					i_data.push_back(it_c->toDouble());

				QTableWidgetItem *table_item = item(row, column);
				if(table_item)
					table_item->setText(*it_c);
				else
				{
					table_item = new QTableWidgetItem(*it_c);
					setItem(row, column, table_item);
				}

				++column;
			}
			++row;
			setColumnCount(column);
			if (it_r != rows.end() - 1)
				column = 0;
			else
				--column;
		}
	}
	calcParams(i_data,i_res, column, row);
	setRowCount(row);

	return params;
}


