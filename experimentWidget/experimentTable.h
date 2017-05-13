#pragma once
#include <QTableWidget>
#include <set>

class ExperimentTable: public QTableWidget
{
	Q_OBJECT
public:
	ExperimentTable(QWidget* parent = 0);
	virtual ~ExperimentTable();
	void setFeatures(const QStringList& features, const std::set<int>& enabledFeatures);
	std::set<int> checkedFeatures() const;

public slots:
	void disableFeature(const QString& str);

signals:
	void featuresChanged();

private:
	QStringList features_;

};
