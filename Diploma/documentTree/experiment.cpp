#include "common/common.h"
#include "experiment.h"

#include "document.h"
#include "regression.h"

Experiment::Experiment(const Document& document)
	: INode("Experiment", "Regression")
	, document_(document)
{

}

Experiment::~Experiment()
{

}

const Dataset& Experiment::getDataset() const
{
	return document_.getDataset();
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
