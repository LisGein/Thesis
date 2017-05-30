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
	void insertedTable(std::string params);
	void addColumn();
	void deleteColumn(int i);

private slots:
	void pasteTable();
	void deleteColumnUnderCursor();

private:
	QShortcut *insertShortcut_;
	QPoint clickedCursor_;
};
