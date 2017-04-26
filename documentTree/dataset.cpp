#include "dataset.h"

#include <sstream>
#include <boost/algorithm/string/split.hpp>


Dataset::Dataset()
	: data_(arma::zeros(3,3))
	, names_({"x1", "x2", "y"})
	, response_(2)
{

}

Dataset::Dataset(const arma::mat& data, const std::vector<std::string>& features)
	: data_(data)
	, names_(features)
	, response_(data_.n_cols)
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

std::vector<std::string> Dataset::getFeatures() const
{
	std::vector<std::string> features = names_;
	features.erase(features.begin() + response_);
	return features;
}

const std::vector<std::string>&Dataset::getNames() const
{
	return names_;
}

const arma::mat& Dataset::data() const
{
	return data_;
}

Dataset Dataset::filterFeatures(const std::set<int>& enabledFeatures) const
{
	arma::mat featureColumns(data_.n_rows, 0);
	for (const auto& feature : enabledFeatures)
	{
		arma::mat featureColumn = data_.col(feature);
		featureColumns.insert_cols(featureColumns.n_cols, featureColumn);
	}

	std::vector<std::string> filteredFeatures;
	for (const auto& id : enabledFeatures )
	{
		filteredFeatures.push_back(names_[id]);
	}

	return Dataset(featureColumns, filteredFeatures);
}

void Dataset::loadFromTsv(const std::string& str)
{
	std::istringstream istr(str);

	std::string headerStr;
	std::getline(istr, headerStr);

	boost::algorithm::split(names_, headerStr, boost::is_any_of("\t"));

	data_.load(istr);
}

void Dataset::setResponse(int response)
{
	response_ = response;
}

const arma::vec Dataset::responses() const
{
	return data_.col(response_);
}

int Dataset::response() const
{
	return response_;
}

