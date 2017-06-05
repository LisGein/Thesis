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
	void set(int row, int column, double data);
	std::vector<std::string> getFeatures() const;
	const std::vector<std::string>& getNames() const;

	const arma::mat& data() const;

	Dataset filterFeatures(const std::set<int>& enabledFeatures) const;

	void loadFromTsv(const std::string& str);

	void saveRegression(boost::property_tree::ptree& inventoryTree);
	void openRegression(boost::property_tree::ptree& dataset);

	const arma::vec getColumnVector(int id) const;
	void addColumn(const std::string &columnName);
	void addRow();
	void renameColumn(size_t column, const std::string& str);
	void deleteColumn(size_t column);
	void deleteRow(size_t row);


private:
	arma::mat data_;
	std::vector<std::string> names_;
};
