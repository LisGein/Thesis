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
public:
	DatasetWidget();
	~DatasetWidget();
	void setDatasetModel(DatasetModel* model);

private:
	std::unique_ptr<Ui::DatasetWidget> ui_;
};
