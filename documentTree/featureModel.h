#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>

#include "mlpack/core.hpp"

class Experiment;
class DatasetColumnsView;

class FeatureGraphicsScene;

class FeatureModel
{
public:
	FeatureModel(const Experiment& experiment);
	~FeatureModel();

    typedef std::pair<int, int> Feature;

	FeatureGraphicsScene* getScene();
    const std::vector<std::string> getRawFeatures() const;
    const std::vector<std::string> getFeatureNames() const;

    const std::set<std::string> getRawNames() const;


    std::string getFeatureName(const Feature& feature, bool nameOne = false) const;


	std::string getResponseName() const;

    int nameToColumn(const std::string& name) const;

	void update();

	void addFeature(const Feature& feature);
	void removeFeature(const Feature& feature);

	const arma::mat data() const;
	const arma::vec responses() const;


private:
	std::unique_ptr<FeatureGraphicsScene> featuresScene_;
    const Experiment& experiment_;
    const DatasetColumnsView& dataset_;
    const DatasetColumnsView& responses_;

    std::set<Feature> featureSet_;
    std::map<Feature, int> featureToColumn_;
    std::map<std::string, Feature> nameToFeature_;
    arma::mat featureColumns_;


    void updateData();
};
