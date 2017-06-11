#pragma once
#include <memory>
#include <QWidget>
#include <QMenu>

#include <boost/property_tree/ptree.hpp>


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
	virtual void openRegression(boost::property_tree::ptree &inventoryTree);
	virtual void saveRegression(boost::property_tree::ptree &inventoryTree);

	void setDatasetModel(DatasetModel* model);

signals:
	void updatedTable(std::string params);

private slots:
	void loadFromXls();
	void loadFromTsv();

private:
	std::unique_ptr<Ui::DatasetWidget> ui_;
};
