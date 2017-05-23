#include "datasetAnalysisTable.h"

#include <documentTree/dataset.h>

#include "mlpack/core.hpp"


DatasetAnalysisTable::DatasetAnalysisTable(QWidget *parent)
    : QTableWidget(parent)
{
    setRowCount(5);
    QStringList verticalHeader;
    verticalHeader.append("Average value");
    verticalHeader.append("Median");
    verticalHeader.append("Standard deviation");
    verticalHeader.append("Variance");
    setVerticalHeaderLabels(verticalHeader);
}

DatasetAnalysisTable::~DatasetAnalysisTable()
{

}

void DatasetAnalysisTable::setFeatures(const QStringList &features, const Dataset& dataset)
{
    features_ = features;
    setColumnCount(features_.size());

    for (int i = 0; i != features_.size(); ++i)
    {
        arma::vec vector = dataset.getColumnVector(i);
        setItem(0, i, new QTableWidgetItem(QString::number(mean(vector))));
        setItem(1, i, new QTableWidgetItem(QString::number(median(vector))));
        setItem(2, i, new QTableWidgetItem(QString::number(stddev(vector))));
        setItem(3, i, new QTableWidgetItem(QString::number(var(vector))));
        //setItem(4, i, new QTableWidgetItem(QString::number(arma::range(vector))));
    }
    setHorizontalHeaderLabels(features);
}
