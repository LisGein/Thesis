#include "common/common.h"
#include "featureModel.h"
#include "featureWidget/featureGraphicsScene.h"
#include "dataset.h"
#include "experiment.h"

FeatureModel::FeatureModel(const Dataset& dataset)
	: featuresScene_(std::make_unique<FeatureGraphicsScene>(*this))
	, dataset_(dataset)
{
	update();
}

FeatureModel::~FeatureModel()
{

}

FeatureGraphicsScene*FeatureModel::getScene()
{
	return featuresScene_.get();
}

const std::vector<std::string> FeatureModel::getRawFeatures() const
{
	std::vector<std::string> res{"1"};

	std::vector<std::string> features = dataset_.getFeatures();
	res.insert(res.end(), features.begin(), features.end());
	return res;
}

const std::vector<std::string> FeatureModel::getFeatureNames() const
{
	std::vector<std::string> names;

	for (const auto& feature : featureSet_)
	{
		names.push_back(getFeatureName(feature));
	}

	return names;
}

std::string FeatureModel::getFeatureName(const Feature& feature, bool nameOne) const
{
	auto raw = getRawFeatures(); //TODO: optimize

	std::string name;
	if (feature.first != 0 && feature.second != 0)
	{
		if (feature.first == feature.second)
			name = raw[feature.first] + "^2";
		else
			name = raw[feature.first] + "*" + raw[feature.second];
	}
	else
	{
		if (feature.first == 0 && feature.second == 0)
			name = nameOne ? "1" : "";
		else if(feature.first == 0)
			name = raw[feature.second];
		else if (feature.second == 0)
			name = raw[feature.first];
	}

	return name;
}

std::string FeatureModel::getResponseName() const
{
	return dataset_.getResponseNames();
}

void FeatureModel::update()
{
	featureSet_.clear();
	featuresScene_->updateTable(dataset_.columnCount());
}

void FeatureModel::addFeature(const Feature& feature)
{
	featureSet_.insert(feature);
}

void FeatureModel::removeFeature(const Feature& feature)
{
	featureSet_.erase(feature);
}

const arma::mat FeatureModel::data() const
{
	arma::mat extendedDataset = dataset_.data();
	extendedDataset.insert_cols(0, arma::ones<arma::vec>(extendedDataset.n_rows));

	arma::mat featureColumns(extendedDataset.n_rows, 0);
	for (const auto& feature : featureSet_)
	{
		arma::mat featureColumn = extendedDataset.col(feature.first) % extendedDataset.col(feature.second); //avoid convertion
		featureColumns.insert_cols(featureColumns.n_cols, featureColumn);
	}

	return featureColumns;
}

const arma::vec FeatureModel::responses() const
{
	return dataset_.responses();
}


