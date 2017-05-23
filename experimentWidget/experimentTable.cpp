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

void ExperimentTable::disableFeature(const QString& str)
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

void ExperimentTable::setFeatures(const QStringList& features, const std::vector<int>& enabledFeatures)
{
	features_ = features;
	setRowCount(features_.size());

    std::set<int> s;// FIXME remove vector to set
    for (auto it : enabledFeatures)
        s.insert(it);

	for (int i = 0; i != features_.size(); ++i)
	{
		setItem(i, 0, new QTableWidgetItem(features_[i]));
		QTableWidgetItem *item = new QTableWidgetItem();


        bool checked = s.find(i) != s.end();
		item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
		setItem(i, 1, item);
	}
}

std::list<int> ExperimentTable::checkedFeatures() const
{
    std::list<int> checkedList;
	for (int i = 0; i != features_.size(); ++i)
	{
		if(item(i, 1)->checkState() == Qt::Checked || item(i, 1)->flags() == Qt::NoItemFlags)
		{
            checkedList.push_back(i);
		}
	}
	return checkedList;
}

