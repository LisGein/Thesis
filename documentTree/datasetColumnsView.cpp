#include "datasetColumnsView.h"
#include "dataset.h"


DatasetColumnsView::DatasetColumnsView(const Dataset &originDataset)
    : originDataset_(originDataset)
{

}

DatasetColumnsView::~DatasetColumnsView()
{

}

void DatasetColumnsView::addFeature(int feature)
{
    auto result = std::find(features_.begin(), features_.end(), feature);
    if (result == features_.end())
        features_.push_back(feature);
}

void DatasetColumnsView::removeFeature(int feature)
{
    if (features_.size() > feature)
    {
        auto it = features_.begin() + feature;
        features_.erase(it);
    }
}

void DatasetColumnsView::addFeatures(const std::list<int> &features)
{
    for (auto it: features)
        addFeature(it);
}

void DatasetColumnsView::clear()
{
    features_.clear();
}

const Dataset &DatasetColumnsView::originDataset() const
{
    return originDataset_;
}

int DatasetColumnsView::originColumns(int filteredId) const
{
    if (features_.size() > filteredId)
        return features_[filteredId];

    return -1;
}

std::vector<std::string> DatasetColumnsView::getFeatureNames() const
{
    std::set<int> featuresIds;
    for (int i = 0; i < features_.size(); ++i)
        featuresIds.insert(features_[i]);

    std::vector<std::string> features;


    std::vector<std::string> res = originDataset_.getNames();
    for (int i = 0; i < res.size(); ++i)
        if (featuresIds.find(i) != featuresIds.end())
            features.push_back(res[i]);

    return features;
}

const arma::vec DatasetColumnsView::getColumnVector(int id) const
{
    int originId = originColumns(id);
    if (originId != - 1)
        return originDataset_.getColumnVector(originId);

    return arma::zeros(1,1);
}

const std::vector<int> &DatasetColumnsView::features() const
{
    return features_;
}
