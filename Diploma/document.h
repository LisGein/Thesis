#pragma once
#include <memory>

class Dataset;
class DatasetModel;
class FeatureModel;
class LinearRegressionModel;

class Document
{
public:
	Document();
	~Document();

	DatasetModel& getDatasetModel();
	Dataset& getDataset();
	FeatureModel& getFeaturesModel();
	LinearRegressionModel& getLinearRegressionModel();

private:
	std::unique_ptr<Dataset> dataset_;
	std::unique_ptr<DatasetModel> datasetModel_;
	std::unique_ptr<FeatureModel> featuresModel_;
	std::unique_ptr<LinearRegressionModel> linearRegressionModel_;
};
