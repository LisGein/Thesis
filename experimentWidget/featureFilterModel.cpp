 #include <common/common.h>
#include "featureFilterModel.h"

#include "documentTree/experiment.h"

FeatureFilterModel::FeatureFilterModel(Experiment &experiment)
	: experiment_(experiment)
{
}

void FeatureFilterModel::setResponse(int response)
{
	beginResetModel();
	experiment_.setResponse(response);
	experiment_.removeFeature(response);
	endResetModel();
}

int FeatureFilterModel::rowCount(const QModelIndex &) const
{
	return experiment_.getDataset().getNames().size();
}

QVariant FeatureFilterModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		return to_qt(experiment_.getDataset().getNames().at(index.row()));

	if (role == Qt::CheckStateRole)
	{
		auto& enabledFeatures = experiment_.filtredDataset().features();
		return (enabledFeatures.find(index.row()) != enabledFeatures.end()) ? Qt::Checked : Qt::Unchecked;
	}
	return QVariant();
}

bool FeatureFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() ||
			role != Qt::CheckStateRole)
		return false;

	if (value == Qt::Checked)
		experiment_.addFeature(index.row());
	else
		experiment_.removeFeature(index.row());

	return true;
}

Qt::ItemFlags FeatureFilterModel::flags(const QModelIndex &index) const
{
	auto flags = QAbstractListModel::flags(index) | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemIsEditable;

	auto& response = experiment_.responses().features();

	if (response.find(index.row()) != response.end())
		flags ^= Qt::ItemFlag::ItemIsEnabled;

	return flags;
}
