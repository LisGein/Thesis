#include "document.h"
#include "dataset.h"
#include "datasetWidget/datasetModel.h"
#include "regression.h"
#include "featureModel.h"

Document::Document()
	: dataset_(std::make_unique<Dataset>())
	, datasetModel_(std::make_unique<DatasetModel>(*dataset_))
	, featuresModel_(std::make_unique<FeatureModel>(*dataset_))
	, linearRegressionModel_(std::make_unique<LinearRegressionModel>(*featuresModel_))
{

}

Document::~Document()
{

}

DatasetModel& Document::getDatasetModel()
{
	return *datasetModel_;
}

Dataset& Document::getDataset()
{
	return *dataset_;
}

LinearRegressionModel& Document::getLinearRegressionModel()
{
	return *linearRegressionModel_;
}

FeatureModel& Document::getFeaturesModel()
{
	return *featuresModel_;
}


