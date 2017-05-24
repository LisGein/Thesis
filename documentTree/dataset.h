#pragma once

#include <vector>
#include <string>

#include "mlpack/core.hpp"

#include <boost/property_tree/ptree.hpp>


class Dataset
{
public:
	Dataset();
	Dataset(const arma::mat& data, const std::vector<std::string>& features);

	int rowCount() const;
	int columnCount() const;

	double get(int row, int column) const;
	std::vector<std::string> getFeatures() const;
	const std::vector<std::string>& getNames() const;

	const arma::mat& data() const;

	Dataset filterFeatures(const std::set<int>& enabledFeatures) const;

	void loadFromTsv(const std::string& str);

    void saveRegression(boost::property_tree::ptree& inventoryTree);
    void openRegression(boost::property_tree::ptree& dataset);

	const arma::vec getColumnVector(int id) const;


private:
	arma::mat data_;
	std::vector<std::string> names_;
};
