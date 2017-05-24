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
    void addFeature(int feature);
    void removeFeature(int feature);

    void setFeatures(const std::list<int>& features);

    void clear();

    const Dataset& originDataset() const;

    int originColumns(int filteredId) const;

    std::vector<std::string> getFeatureNames() const;
    const arma::vec getColumnVector(int id) const;
    const std::vector<int>& features() const;


private:
    const Dataset& originDataset_;
    std::vector<int> features_;
};
