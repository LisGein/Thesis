#pragma once
#include <QAbstractTableModel>


class ExperimentTreeModel: public QAbstractTableModel
{
	Q_OBJECT
public:
	ExperimentTreeModel();
	virtual ~ExperimentTreeModel();

	void beginReset();
	void endReset();

	virtual int rowCount(const QModelIndex &) const ;
	virtual int columnCount(const QModelIndex &) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool event(QEvent * e);

	void setFeatures(const QStringList& feature);

private slots:
	void 	dataChange(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

private:
	QStringList feature_;
	QList<Qt::CheckState> isCheck_;

};
