#include "common/common.h"
#include "datasetWidget.h"
#include "ui_datasetWidget.h"

#include "datasetModel.h"

DatasetWidget::DatasetWidget()
	: ui_(std::make_unique<Ui::DatasetWidget>())
{
	ui_->setupUi(this);
}

DatasetWidget::~DatasetWidget()
{

}

void DatasetWidget::setDatasetModel(DatasetModel* model)
{
	ui_->datasetView->setModel(model);

	QObject::connect(ui_->datasetView, SIGNAL(insertedTable(QString)), this, SIGNAL(insertedTable(QString)));
}
