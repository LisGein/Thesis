#include "tableModel.h"
#include <QFont>
#include <QBrush>
#include <cassert>


TableModel::TableModel(QObject* parent)
	: QAbstractTableModel(parent)
	, rowCount_(5)
	, columnCount_(10)
{

}

TableModel::~TableModel()
{

}

void TableModel::beginReset()
{
	beginResetModel();
	rowCount_ = 0;
	columnCount_ = 0;
	headers_.clear();
	data_.clear();
}

void TableModel::endReset()
{
	endResetModel();
}

int TableModel::rowCount(const QModelIndex&) const
{
	return rowCount_;
}

int TableModel::columnCount(const QModelIndex&) const
{
	return columnCount_;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		int row = index.row();
		int col = index.column();

		auto itData = data_.find(col);
		if (itData == data_.end())
			return QString();

		auto it = itData->find(row);
		if (it == itData->end())
			return QString();

		return it.value();
	}

	return QVariant();
}

void TableModel::setHorizontalHeaderLabels(const QStringList& names)
{
	headers_.clear();
	data_.clear();

	int i = 0;
	for (auto &it: names)
	{
		setHeaderData(i, Qt::Horizontal, QVariant(it), Qt::DisplayRole);
		++i;
	}
	emit headerDataChanged(Qt::Horizontal, 0, names.size() - 1);

}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		auto it = headers_.find(section);
		if (it == headers_.end())
		{
			headers_.insert(section, HeaderData(value, false));
			return true;
		}
	}
	return false;
}


QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		auto it = headers_.find(section);
		if (it != headers_.end())
			return it.value().value;
	}

	return QVariant();
}

void TableModel::setData(int row, int column, const QVariant& value)
{
	if (value.toString().size())
	{
		data_[column][row] = value;
		if (rowCount_ <= row)
			rowCount_ = row + 1;
		if (columnCount_ <= column)
			columnCount_ = column + 1;
	}
}

QVector<double> TableModel::dataFromLines(QPair<QString, QString> parentsData) const
{
	int firstColumn = -1;
	int secondColumn = -1;

	for (auto it = headers_.begin(); it != headers_.end(); ++it)
	{
		QString str = it.value().value.toString();
		if (str == parentsData.first)
			firstColumn = it.key();
		if (str == parentsData.second)
			secondColumn = it.key();
	}
	if (firstColumn == -1 && secondColumn == -1)
		return QVector<double>();

	QVector<double> data;
	if (firstColumn == -1 || secondColumn == -1)
	{
		int column = (firstColumn == -1) ? secondColumn : firstColumn;

		auto iter = data_.find(column);
		if (iter != data_.end())
		{
			for (auto &it: *iter)
				data.push_back(it.toDouble());
		}
	}
	else
	{
		auto iterFirst = data_.find(firstColumn);
		auto iterSecond = data_.find(secondColumn);
		if (iterFirst != data_.end() && iterSecond != data_.end())
		{

			auto it2 = iterSecond->begin();
			for (auto &it: *iterFirst)
			{

				assert(it2 != iterSecond->end());
				if (it2 == iterSecond->end())
					break;
				data.push_back(it.toDouble() * it2.value().toDouble());
				++it2;
			}
		}
	}

	return data;
}





HeaderData::HeaderData(const QVariant& _value, bool _isResponse)
	: value(_value)
	, isResponse(_isResponse)
{

}
