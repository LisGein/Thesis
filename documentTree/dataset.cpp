#include "dataset.h"

#include <sstream>
#include <boost/algorithm/string/split.hpp>


Dataset::Dataset()
	: data_(arma::zeros(3,3))
	, features_({"x1", "x2", "y"})
{

}

int Dataset::rowCount() const
{
	return data_.n_rows;
}

int Dataset::columnCount() const
{
	return data_.n_cols;
}

double Dataset::get(int row, int column) const
{
	return data_.at(row, column);
}

const std::vector<std::string>& Dataset::getFeatures() const
{
	return features_;
}

const arma::mat&Dataset::data() const
{
	return data_;
}

void Dataset::loadFromTsv(const std::string& str)
{
	std::istringstream istr(str);

	std::string headerStr;
	std::getline(istr, headerStr);

	boost::algorithm::split(features_, headerStr, boost::is_any_of("\t"));

	data_.load(istr);
}
