#include "experimentTable.h"


ExperimentTable::ExperimentTable(QWidget* parent)
	: QTableWidget(parent)
{
	setColumnCount(2);
}


ExperimentTable::~ExperimentTable()
{

}

void ExperimentTable::setFeatures(const QStringList& feature)
{
	feature_ = feature;

	int i = 0;
	for (auto &it: feature_)
	{
		setItem(i, 0, new QTableWidgetItem(it));
		QTableWidgetItem *item = new QTableWidgetItem();
		item->setCheckState(Qt::Unchecked);
		setItem(i, 1, item);
		++i;
	}
	setRowCount(feature_.size());
}

QStringList ExperimentTable::checked() const
{
	QStringList checkedList;
	for (int i = 0; i != feature_.size(); ++i)
	{

	}
	return checkedList;
}
