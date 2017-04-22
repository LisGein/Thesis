#include "datasetModel.h"

#include "dataset.h"



DatasetModel::DatasetModel(Dataset& dataset, QObject* parent)
	: QAbstractTableModel(parent)
	, dataset_(dataset)
{

}

DatasetModel::~DatasetModel()
{

}

void DatasetModel::beginReset()
{
	beginResetModel();
}

void DatasetModel::endReset()
{
	endResetModel();
}


int DatasetModel::rowCount(const QModelIndex&) const
{
	return dataset_.rowCount();
}

int DatasetModel::columnCount(const QModelIndex&) const
{
	return dataset_.columnCount();
}

QVariant DatasetModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		return dataset_.get(index.row(), index.column());
	}

	return QVariant();
}


QVariant DatasetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		return dataset_.getFeatures()[section].c_str();
	}

	return QVariant();
}

