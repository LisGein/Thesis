#pragma once
#include <memory>
#include <QWidget>

class DatasetModel;

namespace Ui
{
	class DatasetWidget;
}

class DatasetWidget : public QWidget
{
	Q_OBJECT
public:
	DatasetWidget();
	~DatasetWidget();
	void setDatasetModel(DatasetModel* model);

signals:
	void insertedTable(QString params);

private:
	std::unique_ptr<Ui::DatasetWidget> ui_;
};
