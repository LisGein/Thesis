#pragma once
#include <QAbstractItemModel>
#include <mlpack/methods/linear_regression/linear_regression.hpp>

using namespace mlpack::regression;

struct HeaderData
{
	HeaderData(const QVariant &_value, bool _isResponse);

	QVariant value;
	bool isResponse;
};

class DatasetModel: public QAbstractTableModel
{
public:
	DatasetModel(QObject * parent = 0);
	virtual ~DatasetModel();

	void beginReset();
	void endReset();

	virtual int rowCount(const QModelIndex &) const ;
	virtual int columnCount(const QModelIndex &) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void setHorizontalHeaderLabels(const QStringList& names);
	virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	void setData(int row, int column, const QVariant &value);

	QVector<double> dataFromLines(QPair<QString, QString> parentsData) const;

private:
	int rowCount_;
	int columnCount_;


	QMap<int, HeaderData> headers_;
	QMap<int, QMap<int, QVariant>> data_;

};
