#include "common/common.h"
#include "experiment.h"

#include "document.h"
#include "regression.h"

Experiment::Experiment(const Document& document)
	: INode("Experiment", "Regression")
	, document_(document)
	, enabledFeatures_()
	, response_(0)
{
	filtredDataset_ = document_.getDataset().filterFeatures(enabledFeatures_);
}

Experiment::~Experiment()
{

}

const Dataset& Experiment::getDataset() const
{
	return document_.getDataset();
}

const Dataset& Experiment::getFiltredDataset() const
{
	return filtredDataset_;
}

const std::set<int>& Experiment::getEnabledFeatures() const
{
	return enabledFeatures_;
}

void Experiment::setEnabledFeatures(const std::set<int>& enabled)
{
	enabledFeatures_ = enabled;
	updateFiltredDataset();
}

void Experiment::setResponse(int response)
{
	filtredDataset_.setResponse(response);
}

void Experiment::updateFiltredDataset()
{
	filtredDataset_ = document_.getDataset().filterFeatures(enabledFeatures_);
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

INode::TypeObject Experiment::type() const
{
	return TypeObject::Experiment;
}
