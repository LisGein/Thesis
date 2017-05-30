#pragma once
#include <QTableView>
#include <QShortcut>


class DatasetModel;

class DatasetView : public QTableView
{
	Q_OBJECT

public:
	DatasetView(QWidget *parent = 0);
	virtual ~DatasetView();
	virtual void contextMenuEvent(QContextMenuEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void setModel(DatasetModel *model);

signals:
	void insertedTable(std::string params);
	void addColumn();
	void addRow();
	void deleteColumn(int i);
	void renameColumn(int i);

private slots:
	void pasteTable();
	void deleteColumnUnderCursor();
	void renameColumnUnderCursor();
	void addedColumn();
	void addedRow();

private:
	QShortcut* insertShortcut_;
	QPoint clickedCursor_;
	DatasetModel* model_;
};
