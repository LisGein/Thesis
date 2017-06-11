#include "common/common.h"
#include "experiment.h"
#include "document.h"
#include "regression.h"
#include <QObject>


Experiment::Experiment(const Document& document)
	: INode(QObject::tr("Experiment").toStdString(), QObject::tr("Regression").toStdString())
	, document_(document)
	, filtredDataset_(document.getDataset())
	, response_(document.getDataset())
{
	int originalFeaturesCount = getDataset().data().n_cols;
	for (int i = 0; i < originalFeaturesCount - 1; ++i)
	{
		filtredDataset_.addFeature(i);
	}

	response_.addFeature(originalFeaturesCount - 1);
}

Experiment::~Experiment()
{

}

const Dataset& Experiment::getDataset() const
{
	return document_.getDataset();
}

const DatasetColumnsView& Experiment::filtredDataset() const
{
	return filtredDataset_;
}

const DatasetColumnsView &Experiment::responses() const
{
	return response_;
}

void Experiment::openRegression(boost::property_tree::ptree &inventoryTree)
{
	removeAllChilds();

	namespace pt = boost::property_tree;
	pt::ptree filtredArray = inventoryTree.get_child("filtredArray");
	filtredDataset_.clear();

	for(auto &it:filtredArray)
	{
		std::string respCol = it.first;

		setResponse(atoi(respCol.c_str()));
		pt::ptree feature = it.second;
		for(auto &iter:feature)
			filtredDataset_.addFeature(iter.second.get<int>("", 0));
	}

	pt::ptree regressions = inventoryTree.get_child("regressions");
	for(auto &it:regressions)
	{
		addNewChild();
		if (regressions_.empty())
			throw "Application fatal error: don't exist child regression";

		auto &regression = regressions_.back();
		regression->openRegression(it.second);
	}
}

void Experiment::saveRegression(boost::property_tree::ptree &experimentTree)
{
	namespace pt = boost::property_tree;

	auto filtredFeatures = filtredDataset_.features();

	pt::ptree experiment;

	pt::ptree feature;
	for (auto &it : filtredFeatures)
	{
		pt::ptree cell;
		cell.put_value(it);
		feature.push_back(std::make_pair("", cell));
	}

	experiment.push_back(std::make_pair(boost::lexical_cast<std::string>(getResponseColumn()), feature));
	experimentTree.add_child("filtredArray", experiment);

	boost::property_tree::ptree regressions;
	int index = 0;
	for (auto &it: regressions_)
	{
		boost::property_tree::ptree regression;
		std::string name = "regression" + boost::lexical_cast<std::string>(index);
		it->saveRegression(regression);
		regressions.add_child(name, regression);
		++index;
	}
	experimentTree.add_child("regressions", regressions);

}


void Experiment::setResponse(int response)
{
	response_.clear();
	response_.addFeature(response);
}

void Experiment::addFeature(int feature)
{
	filtredDataset_.addFeature(feature);
}

void Experiment::removeFeature(int feature)
{
	filtredDataset_.removeFeature(feature);
}

void Experiment::update()
{
	regressions_.clear();
}

int Experiment::childCount() const
{
	return regressions_.size();
}

INode*Experiment::child(int id)
{
	return regressions_[id].get();
}

const INode* Experiment::parentItem() const
{
	return &document_;
}

void Experiment::addNewChild()
{
	regressions_.emplace_back(std::make_unique<Regression>(*this));
}

void Experiment::removeAllChilds()
{
	regressions_.clear();
}

INode::TypeObject Experiment::type() const
{
	return TypeObject::Experiment;
}
