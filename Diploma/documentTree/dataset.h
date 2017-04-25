#pragma once

#include <vector>
#include <string>

#include "mlpack/core.hpp"

class Dataset
{
public:
	Dataset();

	int rowCount() const;
	int columnCount() const;

	double get(int row, int column) const;
	const std::vector<std::string>& getFeatures() const;

	const arma::mat& data() const;

	void loadFromTsv(const std::string& str);

private:
	arma::mat data_;
	std::vector<std::string> features_;
};
