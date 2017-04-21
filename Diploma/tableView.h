#pragma once
#include <QTableView>
#include <QShortcut>

class TableModel;

class TableView : public QTableView
{
	Q_OBJECT

public:
	TableView(QWidget *parent = 0);
	virtual ~TableView();



	virtual void contextMenuEvent(QContextMenuEvent *event) override;


	QVector<double> dataFromLines(const QString& first, const QString& second) const;

signals:
	void insertedTable(QStringList params);

private slots:
	void pasteTable();

private:
	const QStringList parseTable(const QString& str);

	TableModel* model_;
	QShortcut *insertShortcut_;

};
