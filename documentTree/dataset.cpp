#include "dataset.h"

#include <sstream>
#include <boost/algorithm/string/split.hpp>


Dataset::Dataset()
	: data_(arma::zeros(1,1))
	, names_({"Insert data"})
{

}

Dataset::Dataset(const arma::mat& data, const std::vector<std::string>& features)
	: data_(data)
	, names_(features)
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
	std::vector<std::string> filteredFeatures;

	for (const auto& feature : enabledFeatures)
	{
		arma::mat featureColumn = data_.col(feature);
		featureColumns.insert_cols(featureColumns.n_cols, featureColumn);

		filteredFeatures.push_back(names_[feature]);

	}


	Dataset res(featureColumns, filteredFeatures);
	return res;
}

void Dataset::loadFromTsv(const std::string& str)
{
	std::istringstream istr(str);

	std::string headerStr;
	std::getline(istr, headerStr);

	boost::algorithm::split(names_, headerStr, boost::is_any_of("\t"));

	data_.load(istr);
}

const arma::vec Dataset::getColumnVector(int id) const
{
	return data_.col(id);
}

