#include "common/common.h"
#include "document.h"
#include "dataset.h"
#include "datasetWidget/datasetModel.h"
#include "experiment.h"

#include <boost/lexical_cast.hpp>


Document::Document()
	: INode(QObject::tr("Document").toStdString(), QObject::tr("Experiment").toStdString())
	, dataset_(std::make_unique<Dataset>())
	, datasetModel_(std::make_unique<DatasetModel>(*dataset_))
{

}

Document::~Document()
{

}

DatasetModel& Document::getDatasetModel()
{
	return *datasetModel_;
}

Dataset& Document::getDataset()
{
	return *dataset_;
}

const Dataset& Document::getDataset() const
{
	return const_cast<Document*>(this)->getDataset();
}

void Document::loadFromTsv(const std::string& tsv)
{
	datasetModel_->beginReset();
	dataset_->loadFromTsv(tsv);
	datasetModel_->endReset();

	for (const auto& experiment : experiments_)
	{
		experiment->update();
	}
}

void Document::openRegression(boost::property_tree::ptree &inventoryTree)
{
	removeAllChilds();

	datasetModel_->beginReset();
	dataset_->openRegression(inventoryTree.get_child("dataset"));
	datasetModel_->endReset();

	namespace pt = boost::property_tree;
	pt::ptree experiments = inventoryTree.get_child("experiments");
	for(auto &it:experiments)
	{
		addNewChild();
		auto experiment = experiments_.end() - 1;
		if (experiment == experiments_.end())
			throw "Application fatal error: don't exist child experiment";

		(*experiment)->openRegression(it.second);
	}
}

void Document::saveRegression(boost::property_tree::ptree &inventoryTree)
{
	boost::property_tree::ptree dataset;
	dataset_->saveRegression(dataset);
	inventoryTree.add_child("dataset", dataset);

	int index = 0;

	boost::property_tree::ptree experiments;
	for (auto &it: experiments_)
	{
		boost::property_tree::ptree experiment;
		it->saveRegression(experiment);
		std::string name = "experiment" + boost::lexical_cast<std::string>(index);
		experiments.add_child(name, experiment);
		++index;
	}
	inventoryTree.add_child("experiments", experiments);
}



int Document::childCount() const
{
	return experiments_.size();
}

INode* Document::child(int id)
{
	return experiments_[id].get();
}

const INode* Document::parentItem() const
{
	return nullptr;
}

void Document::addNewChild()
{
	experiments_.emplace_back(std::make_unique<Experiment>(*this));
}

void Document::removeAllChilds()
{
	experiments_.clear();
}

INode::TypeObject Document::type() const
{
	return TypeObject::Document;
}
