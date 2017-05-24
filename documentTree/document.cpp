#include "document.h"
#include "dataset.h"
#include "datasetWidget/datasetModel.h"
#include "experiment.h"


Document::Document()
    : INode("Document", "Experiment")
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
    datasetModel_->beginReset();
    dataset_->openRegression(inventoryTree.get_child("dataset"));
    datasetModel_->endReset();

    for (const auto& experiment : experiments_)
    {
        experiment->update();
    }
}

void Document::saveRegression(boost::property_tree::ptree &inventoryTree)
{
    boost::property_tree::ptree dataset;
    dataset_->saveRegression(dataset);
    inventoryTree.add_child("dataset", dataset);

    int index = 0;
    for (auto &it: experiments_)
    {
        boost::property_tree::ptree experiment;
        it->saveRegression(experiment);
        std::string name = "experiment" + boost::lexical_cast<std::string>(index);
        inventoryTree.add_child(name, experiment);
        ++index;
    }
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

INode::TypeObject Document::type() const
{
    return TypeObject::Document;
}
