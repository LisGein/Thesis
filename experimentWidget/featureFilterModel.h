#pragma once

#include <QAbstractListModel>

class Experiment;

class FeatureFilterModel : public QAbstractListModel
{
public:
	FeatureFilterModel(Experiment& experiment);
public:
	void setResponse(int response);
	// QAbstractItemModel interface
public:
	virtual int rowCount(const QModelIndex &parent) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

	Experiment& experiment_;

};
