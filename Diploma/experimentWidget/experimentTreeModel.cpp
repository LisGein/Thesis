#include "experimentTreeModel.h"


ExperimentTreeModel::ExperimentTreeModel()
	: QAbstractTableModel(nullptr)
{
	QObject::connect(this,
						  SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &))
						  , this, SLOT(dataChange(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
}

ExperimentTreeModel::~ExperimentTreeModel()
{
}

void ExperimentTreeModel::beginReset()
{
	beginResetModel();
}

void ExperimentTreeModel::endReset()
{
	endResetModel();
}

int ExperimentTreeModel::rowCount(const QModelIndex&) const
{
	return feature_.count();
}

int ExperimentTreeModel::columnCount(const QModelIndex&) const
{
	return 2;
}

QVariant ExperimentTreeModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int col = index.column();


	switch(role)
	{
	case Qt::DisplayRole:
		if (col == 0 && row < feature_.size())
			return feature_[row];

		break;
	case Qt::CheckStateRole:

		if (col == 1 && row < feature_.size())
		{
			return isCheck_[row];
		}
	}
	return QVariant();
}

QVariant ExperimentTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if (section == 0)
			return QString("Feature");
		else if (section == 1)
			return QString("Check");
	}
	return QVariant();
}

Qt::ItemFlags ExperimentTreeModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if (index.column() == 1 && index.row() < feature_.size())
		f |= Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;

	return f;
}

bool ExperimentTreeModel::event(QEvent* e)
{
	return QAbstractTableModel::event(e);
}

void ExperimentTreeModel::setFeatures(const QStringList& feature)
{
	feature_ = feature;
	isCheck_.clear();
	isCheck_.reserve(feature_.size());
	for (auto &it : feature_)
		isCheck_.append(Qt::Unchecked);

}

void ExperimentTreeModel::dataChange(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
	int row = topLeft.row();
	int col = topLeft.column();
	int row2 = bottomRight.row();
	int col2 = bottomRight.column();
	col2 = bottomRight.column();

}
