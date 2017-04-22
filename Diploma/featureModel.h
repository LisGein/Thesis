#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>

#include "mlpack/core.hpp"

class Dataset;

class FeatureGraphicsScene;

class FeatureModel
{
public:
	FeatureModel(const Dataset& dataset);
	~FeatureModel();

	typedef std::pair<int, int> Feature;

	FeatureGraphicsScene* getScene();
	const std::vector<std::string> getRawFeatures() const;
	const std::vector<std::string> getFeatureNames() const;
	std::string getFeatureName(const Feature& feature, bool nameOne = false) const;
	std::string getResponseName() const;

	void update();

	void addFeature(const Feature& feature);
	void removeFeature(const Feature& feature);

	const arma::mat data() const;
	const arma::vec responses() const;

private:
	std::unique_ptr<FeatureGraphicsScene> featuresScene_;
	const Dataset& dataset_;

	std::set<Feature> featureSet_;
	int response_;
};
