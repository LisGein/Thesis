#include "common/common.h"
#include "featureModel.h"
#include "featureWidget/featureGraphicsScene.h"
#include "datasetColumnsView.h"
#include "experiment.h"


FeatureModel::FeatureModel(const Experiment& experiment)
	: featuresScene_(std::make_unique<FeatureGraphicsScene>(*this))
	, experiment_(experiment)
	, dataset_(experiment_.filtredDataset())
	, responses_(experiment_.responses())
{
	update();
}

FeatureModel::~FeatureModel()
{

}

FeatureGraphicsScene* FeatureModel::getScene()
{
	return featuresScene_.get();
}

const std::vector<std::string> FeatureModel::getRawFeatures() const
{
	std::vector<std::string> res{"1"};

	auto features = dataset_.getFeatureNames();
	res.insert(res.end(), features.begin(), features.end());
	auto responses = responses_.getFeatureNames();
	res.insert(res.end(), responses.begin(), responses.end());
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

const std::set<int> FeatureModel::getRawIds() const
{
	std::set<int> raws;
	for (const auto& feature : featureSet_)
	{
		if (feature.first != 0 )
			raws.insert(feature.first);
		if (feature.second != 0)
			raws.insert(feature.second);
	}

	return raws;
}

std::string FeatureModel::idTofeatureName(int id) const
{
	if (id == 0)
		return std::string();

	auto raw = getRawFeatures();
	return raw[id];
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
	return responses_.getFeatureNames()[0];
}

int FeatureModel::nameToOriginColumn(const std::string &name) const
{
	Feature itFeature = nameToFeature(name);
	if (itFeature.first == -1 && itFeature.second == -1)
		return -1;

	return featureToOriginColumn(itFeature);
}

int FeatureModel::featureToOriginColumn(const Feature &feature) const
{
	auto itColumn = featureToColumn_.find(feature);
	if (itColumn == featureToColumn_.end())
		return -1;

	return itColumn->second;
}

const FeatureModel::Feature FeatureModel::nameToFeature(const std::string &srt) const
{
	auto itFeature = nameToFeature_.find(srt);
	if (itFeature == nameToFeature_.end())
		return std::pair<int, int>(-1, -1);
	return itFeature->second;
}

void FeatureModel::update()
{
	featureSet_.clear();
	nameToFeature_.clear();
	updateData();
	featuresScene_->updateTable(dataset_.features().size() + 1);
}

void FeatureModel::addFeature(const Feature& feature)
{
	featureSet_.insert(feature);

	auto raw = getRawFeatures(); //TODO: optimize

	std::string name;
	if (feature.first != 0 && feature.second != 0)
	{
		if (feature.first == feature.second)
		{
			name = raw[feature.first] + "^2";
			nameToFeature_.emplace(name, feature);
			nameToFeature_.emplace(raw[feature.first], Feature(feature.first, 0));
		}
		else
		{
			name = raw[feature.first] + "*" + raw[feature.second];
			nameToFeature_.emplace(name, feature);
			nameToFeature_.emplace(raw[feature.first], Feature(feature.first, 0));
			nameToFeature_.emplace(raw[feature.second], Feature(feature.second, 0));
		}
	}
	else
	{
		if(feature.first == 0)
		{
			name = raw[feature.second];
			nameToFeature_.emplace(raw[feature.second], Feature(feature.second, 0));
		}
		else if (feature.second == 0)
		{
			name = raw[feature.first];
			nameToFeature_.emplace(raw[feature.first], Feature(feature.first, 0));
		}
	}
	updateData();
}

void FeatureModel::removeFeature(const Feature& feature)
{
	featureSet_.erase(feature);
	nameToFeature_.erase(getFeatureName(feature));
	updateData();
}

const arma::mat FeatureModel::data() const
{
	return featureColumns_;
}

const arma::vec FeatureModel::responses() const
{
	return responses_.getColumnVector(0);
}
const arma::mat FeatureModel::originData() const
{
	return dataset_.originDataset().data();
}

const std::set<FeatureModel::Feature> &FeatureModel::featureSet() const
{
	return featureSet_;
}

arma::vec FeatureModel::getFinalFeaturesValue(const std::map<int, double> &rawFeaturesValue) const
{
	arma::vec res(featureSet_.size());

	auto extendedRawFeaturesValue = rawFeaturesValue;
	extendedRawFeaturesValue[0] = 1.;

	int i = 0;
	for (const auto& feature : featureSet_)
	{
		auto itFirst = extendedRawFeaturesValue.find(feature.first);
		if (itFirst != extendedRawFeaturesValue.end())
		{
			auto itSecond = extendedRawFeaturesValue.find(feature.second);
			if (itSecond != extendedRawFeaturesValue.end())
			{
				res[i] = itFirst->second * itSecond->second;
				++i;
			}
		}
	}

	return res;
}

const arma::vec FeatureModel::columnAt(Feature feature) const
{
	arma::mat extendedDataset = dataset_.originDataset().data();
	extendedDataset.insert_cols(0, arma::ones<arma::vec>(extendedDataset.n_rows));
	return arma::vec(extendedDataset.col(feature.first) % extendedDataset.col(feature.second));
}


void FeatureModel::updateData()
{
	arma::mat extendedDataset = dataset_.originDataset().data();
	extendedDataset.insert_cols(0, arma::ones<arma::vec>(extendedDataset.n_rows));

	featureColumns_ = arma::mat(extendedDataset.n_rows, 0);
	int column = 0;
	for (const auto& feature : featureSet_)
	{
		arma::mat featureColumn = extendedDataset.col(feature.first) % extendedDataset.col(feature.second); //avoid convertion
		featureColumns_.insert_cols(featureColumns_.n_cols, featureColumn);
		featureToColumn_.emplace(feature, column);
		++column;
	}
}

