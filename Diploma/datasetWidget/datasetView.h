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


	QVector<double> dataFromLines(const QString& first, const QString& second) const;

signals:
	void insertedTable(QStringList params);

private slots:
	void pasteTable();

private:
	const QStringList parseTable(const QString& str);

	DatasetModel* model_;
	QShortcut *insertShortcut_;

};
