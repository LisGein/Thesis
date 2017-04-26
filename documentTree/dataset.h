#pragma once

#include <vector>
#include <string>

#include "mlpack/core.hpp"

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

	void setResponse(int response);
	const arma::vec responses() const;
	int response() const;

private:
	arma::mat data_;
	std::vector<std::string> names_;
	int response_;

};
