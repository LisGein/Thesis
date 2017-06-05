#include "datasetModel.h"

#include "documentTree/dataset.h"

#include <QInputDialog>


DatasetModel::DatasetModel(Dataset& dataset, QObject* parent)
	: QAbstractTableModel(parent)
	, dataset_(dataset)
	, lastEditable_(0, 0)
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
	if (role == Qt::EditRole)
	{
		return  dataset_.get(index.row(), index.column());
	}
	else if(role == Qt::DisplayRole)
		return  dataset_.get(index.row(), index.column());

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

Qt::ItemFlags DatasetModel::flags(const QModelIndex &index) const
{
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool DatasetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role == Qt::EditRole)
	{
		lastEditable_ = QPair<int, int>(index.row(), index.column());
		dataset_.set(index.row(), index.column(), value.toDouble());
	}
	return QAbstractTableModel::setData(index, value, role);
}

const QPair<int, int> &DatasetModel::lastEditable() const
{
	return lastEditable_;
}

void DatasetModel::addColumn(const QString &text)
{
	beginResetModel();
	dataset_.addColumn(text.toStdString());
	endResetModel();
}

void DatasetModel::addRow()
{
	beginResetModel();
	dataset_.addRow();
	endResetModel();
}

void DatasetModel::deleteColumn(int i)
{
	beginResetModel();
	try
	{
		dataset_.deleteColumn(i);
	}
	catch(std::string& str)
	{
		std::cout << str << std::endl;
	}
	endResetModel();
}

void DatasetModel::deleteRow(int i)
{
	beginResetModel();
	try
	{
		dataset_.deleteRow(i);
	}
	catch(std::string& str)
	{
		std::cout << str << std::endl;
	}
	endResetModel();
}

void DatasetModel::renameColumn(int i, const QString &text)
{
	beginResetModel();
	try
	{
		dataset_.renameColumn(i, text.toStdString());
	}
	catch(std::string& str)
	{
		std::cout << str << std::endl;
	}
	endResetModel();
}
