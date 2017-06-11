#pragma once

#include <list>
#include <vector>
#include <string>

#include "mlpack/core.hpp"


class Dataset;

class DatasetColumnsView
{
public:
	DatasetColumnsView(const Dataset& originDataset);
	~DatasetColumnsView();
	void setFeatures(const std::set<int>& features);
	void addFeature (int feature);
	void removeFeature (int feature);
	void clear();

	const Dataset& originDataset() const;

	int originColumns(size_t filteredId) const;

	std::vector<std::string> getFeatureNames() const;
	const arma::vec getColumnVector(int id) const;
	const std::set<int> &features() const;


private:
	const Dataset& originDataset_;
	std::set<int> features_;
};
