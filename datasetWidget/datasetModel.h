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
	Q_OBJECT
public:
	DatasetModel(Dataset& dataset, QObject * parent = 0);
	virtual ~DatasetModel();

	void beginReset();
	void endReset();

	virtual int rowCount(const QModelIndex &) const ;
	virtual int columnCount(const QModelIndex &) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags (const QModelIndex &index) const;
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

	const QPair<int, int> &lastEditable() const;

	void deleteColumn(int i);
	void deleteRow(int i);
	void renameColumn(int i, const QString &text);
	void addColumn(const QString &text);

public slots:
	void addRow();

private:
	Dataset& dataset_;
	QPair<int, int> lastEditable_;
};
