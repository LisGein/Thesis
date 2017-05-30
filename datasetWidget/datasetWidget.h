#pragma once
#include <memory>
#include <QWidget>
#include <QMenu>


class DatasetModel;
class QMenuBar;
class DatasetView;
class QTableView;

class DatasetWidget : public QWidget
{
	Q_OBJECT
public:
	DatasetWidget();
	~DatasetWidget();
	void setDatasetModel(DatasetModel* model);

signals:
	void updatedTable(std::string params);

private slots:
	void loadFromXls();
	void loadFromTsv();

private:
	QMenuBar *menu_;
	DatasetView *datasetView_;
	QTableView *featureParams_;
};
