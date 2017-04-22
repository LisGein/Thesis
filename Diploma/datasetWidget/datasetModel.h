#pragma once
#include <QAbstractTableModel>
#include <mlpack/methods/linear_regression/linear_regression.hpp>

using namespace mlpack::regression;

class Dataset;

struct HeaderData
{
	HeaderData(const QVariant &_value, bool _isResponse);

	QVariant value;
	bool isResponse;
};

class DatasetModel: public QAbstractTableModel
{
public:
	DatasetModel(Dataset& dataset, QObject * parent = 0);
	virtual ~DatasetModel();

	void beginReset();
	void endReset();

	virtual int rowCount(const QModelIndex &) const ;
	virtual int columnCount(const QModelIndex &) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;


	Dataset& dataset_;
};
