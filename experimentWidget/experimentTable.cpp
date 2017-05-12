#include "experimentTable.h"


ExperimentTable::ExperimentTable(QWidget* parent)
	: QTableWidget(parent)
{
	setColumnCount(2);
	QObject::connect(this, SIGNAL(itemActivated(QTableWidgetItem*)), this, SIGNAL(featuresChanged()));
}


ExperimentTable::~ExperimentTable()
{

}

void ExperimentTable::removeItem(const QString& str)
{
	for (int i = 0; i != features_.size(); ++i)
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

void ExperimentTable::setFeatures(const QStringList& features, const std::set<int>& enabledFeatures)
{
	features_ = features;
	setRowCount(features_.size());

	for (int i = 0; i != features_.size(); ++i)
	{
		setItem(i, 0, new QTableWidgetItem(features_[i]));
		QTableWidgetItem *item = new QTableWidgetItem();


		bool checked = true;//enabledFeatures.find(i) != enabledFeatures.end();
		item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
		setItem(i, 1, item);
	}
}

std::pair<int, std::set<int> > ExperimentTable::checkedFeatures() const
{
	std::set<int> checkedList;
	int count = 0;
	int response = 0;
	for (int i = 0; i != features_.size(); ++i)
	{
		if(item(i, 1)->checkState() == Qt::Checked || item(i, 1)->flags() == Qt::NoItemFlags)
		{
			checkedList.insert(i);
			if (item(i, 1)->flags() == Qt::NoItemFlags)
				response = count;
			++count;
		}
	}
	return std::make_pair(response, checkedList);
}

