#include "table.h"
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
	QObject::connect(insert_shortcut_, SIGNAL(activated()), this, SLOT(paste_table()));
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

void TableWidget::paste_table()
{
	QString clipboard = QApplication::clipboard()->text();
	qDebug() << clipboard;
	parse_table(clipboard.trimmed());
}

void TableWidget::parse_table(const QString &str)
{
	QStringList rows = str.split("\n");

	int row = 0;
	int column = 0;


	for(auto it_r = rows.begin(); it_r != rows.end(); ++ it_r)
	{
		QStringList columns = it_r->split("\t");

		if (it_r == rows.begin())
			setHorizontalHeaderLabels(columns);
		else
		{
			for(auto &it_c: columns)
			{
				QTableWidgetItem *table_item = item(row, column);
				if(table_item)
					table_item->setText(it_c);
				else
				{
					table_item = new QTableWidgetItem(it_c);
					setItem(row, column, table_item);
				}

				++column;
			}
			++row;
			setColumnCount(column);
			column = 0;
		}
	}
	setRowCount(row);
}


