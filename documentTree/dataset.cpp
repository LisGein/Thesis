#include "dataset.h"

#include <sstream>

#include <boost/algorithm/string/split.hpp>

#include <QDebug>


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

void Dataset::saveRegression(boost::property_tree::ptree &dataset)
{
    namespace pt = boost::property_tree;

    {
        pt::ptree info_node;
        info_node.put("columns", data_.n_cols);
        info_node.put("rows", data_.n_rows);
        dataset.add_child("info", info_node);
    }

    {
        pt::ptree matrix_node;
        for(size_t c = 0; c < data_.n_cols; ++c)
        {
            pt::ptree row;
            for (int r = 0; r < data_.n_rows; ++r)
            {
                pt::ptree cell;
                cell.put_value(data_.at(r, c));
                row.push_back(std::make_pair("", cell));
            }
            matrix_node.push_back(std::make_pair(names_[c], row));
        }

        dataset.add_child("matrix", matrix_node);
    }
}

void Dataset::openRegression(boost::property_tree::ptree &dataset)
{
    names_.clear();


    namespace pt = boost::property_tree;

    pt::ptree info = dataset.get_child("info");
    int columns = info.get<int>("columns", 0);
    int rows = info.get<int>("rows", 0);


    data_ = arma::zeros(rows, columns);

    pt::ptree matrix = dataset.get_child("matrix");

    int c = 0;
    for (pt::ptree::value_type &row : matrix)
    {
        if (c >= columns) return;

        names_.push_back(row.first);

        int r = 0;
        for (pt::ptree::value_type &cell : row.second)
        {
            if (r >= rows) return;
            data_(r, c) = cell.second.get_value<double>();
            r++;
        }
        c++;
    }
}

const arma::vec Dataset::getColumnVector(int id) const
{
    return data_.col(id);
}

