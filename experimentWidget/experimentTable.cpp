#include "experimentTable.h"


ExperimentTable::ExperimentTable(QWidget* parent)
	: QTableWidget(parent)
{
	setColumnCount(2);
}


ExperimentTable::~ExperimentTable()
{

}

void ExperimentTable::removeItem(const QString& str)
{
	for (int i = 0; i != feature_.size(); ++i)
	{
		QTableWidgetItem * it = item(i, 1);
		if(item(i, 0)->text() == str)
		{
			it->setCheckState(Qt::Unchecked);
			it->setFlags(Qt::NoItemFlags);
		}
		else
			it->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

	}
}

void ExperimentTable::setFeatures(const QStringList& feature)
{
	feature_ = feature;
	setRowCount(feature_.size());


	int i = 0;
	for (auto &it: feature_)
	{
		setItem(i, 0, new QTableWidgetItem(it));
		QTableWidgetItem *item = new QTableWidgetItem();
		item->setCheckState(Qt::Unchecked);
		setItem(i, 1, item);
		++i;
	}
}

QStringList ExperimentTable::checked() const
{
	QStringList checkedList;
	for (int i = 0; i != feature_.size(); ++i)
	{
		if(item(i, 1)->checkState() == Qt::Checked)
			checkedList.append(item(i, 0)->text());
	}
	return checkedList;
}
