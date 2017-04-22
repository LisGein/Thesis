#pragma once
#include <QTableView>
#include <QShortcut>

class DatasetView : public QTableView
{
	Q_OBJECT

public:
	DatasetView(QWidget *parent = 0);
	virtual ~DatasetView();

	virtual void contextMenuEvent(QContextMenuEvent *event) override;


signals:
	void insertedTable(QString params);

private slots:
	void pasteTable();

private:
	QShortcut *insertShortcut_;

};
