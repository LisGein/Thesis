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

    const std::set<int> getRawIds() const;
    void getFinalColumnIdsByRow();



    int nameToOriginColumn(const std::string& name) const;
    int featureToOriginColumn(const Feature& feature) const;
    const Feature nameToFeature(const std::string &srt) const;
    std::string idTofeatureName(int id) const;

    std::string getFeatureName(const Feature& feature, bool nameOne = false) const;
    std::string getResponseName() const;

	void update();

	void addFeature(const Feature& feature);
	void removeFeature(const Feature& feature);

    const arma::mat data() const;
    const arma::vec responses() const;

    const arma::vec columnAt(Feature feature) const;

    const arma::mat originData() const;
    const std::set<Feature>& featureSet() const;

    arma::vec getFinalFeaturesValue(const std::map<int, double> &rawFeaturesValue) const;

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
