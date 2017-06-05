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
	void deleteColumn(int i);
	void renameColumn(int i);

private slots:
	void pasteTable();
	void addColumn();
	void deleteColumnUnderCursor();
	void deleteRowUnderCursor();
	void renameColumnUnderCursor();

	void headerContextMenu(const QPoint &point);

private:
	void generateMenu(QMenu &menu);

	QShortcut* insertShortcut_;
	QPoint clickedCursor_;
	DatasetModel* model_;

	QAction *columnAdd_;
	QAction *rowAdd_;
	QAction *deleteCol_;
	QAction *deleteRow_;
	QAction *renameCol_;
};
