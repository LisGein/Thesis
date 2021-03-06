#include "datasetAnalysisTable.h"

#include <documentTree/dataset.h>

#include <armadillo>


DatasetAnalysisTable::DatasetAnalysisTable(QWidget *parent)
	: QTableWidget(parent)
{
	setRowCount(6);
	QStringList verticalHeader;
	verticalHeader.append(QObject::tr("Average value"));
	verticalHeader.append(QObject::tr("Median"));
	verticalHeader.append(QObject::tr("Standard deviation"));
	verticalHeader.append(QObject::tr("Variance"));
	verticalHeader.append(QObject::tr("Minimum"));
	verticalHeader.append(QObject::tr("Maximum"));
	//verticalHeader.append(QObject::tr("Criterion of informativity"));
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
		setItem(4, i, new QTableWidgetItem(QString::number(min(vector))));
		setItem(5, i, new QTableWidgetItem(QString::number(max(vector))));
	}
	setHorizontalHeaderLabels(features);
}
