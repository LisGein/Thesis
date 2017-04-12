#pragma once
#include <QTableWidget>

#include <QShortcut>

class TableWidget : public QTableWidget
{
	 Q_OBJECT

public:
	 TableWidget(QWidget *parent = 0);
	 virtual ~TableWidget();

	 virtual void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
	 void paste_table();

private:
	 void parse_table(const QString& str);

		QShortcut *insert_shortcut_;
};
