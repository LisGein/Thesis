#include "common/common.h"
#include "datasetColumnsView.h"
#include "dataset.h"


DatasetColumnsView::DatasetColumnsView(const Dataset &originDataset)
	: originDataset_(originDataset)
{

}

DatasetColumnsView::~DatasetColumnsView()
{

}


void DatasetColumnsView::setFeatures(const std::set<int> &features)
{
	features_ = features;
}

void DatasetColumnsView::addFeature(int feature)
{
	features_.insert(feature);
}

void DatasetColumnsView::removeFeature(int feature)
{
	features_.erase(feature);
}

void DatasetColumnsView::clear()
{
	features_.clear();
}

const Dataset &DatasetColumnsView::originDataset() const
{
	return originDataset_;
}

int DatasetColumnsView::originColumns(size_t filteredId) const
{
	if (filteredId >= features_.size())
		throw std::runtime_error("unexpected filtered column");

	return *std::next(features_.begin(), filteredId);
}

std::vector<std::string> DatasetColumnsView::getFeatureNames() const
{
	std::vector<std::string> names;

	for (int feature : features_)
		names.push_back(originDataset_.getNames()[feature]);

	return names;
}

const arma::vec DatasetColumnsView::getColumnVector(int id) const
{
	int originId = originColumns(id);
	if (originId != - 1)
		return originDataset_.getColumnVector(originId);

	throw std::runtime_error("unexpected filtered column");
}

const std::set<int> &DatasetColumnsView::features() const
{
	return features_;
}
