#pragma once
#include <QTableWidget>


class ExperimentTable: public QTableWidget
{
	Q_OBJECT
public:
	ExperimentTable(QWidget* parent = 0);
	virtual ~ExperimentTable();

	void setFeatures(const QStringList& feature);

	QStringList checked() const;

private:
	QStringList feature_;

};
