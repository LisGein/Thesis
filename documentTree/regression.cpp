#include "common/common.h"
#include "regression.h"

#include "experiment.h"

int Regression::childCount() const
{
	return 0;
}

INode* Regression::child(int id)
{
	Q_UNUSED(id)

	return nullptr;
}

const INode* Regression::parentItem() const
{
	return &experiment_;
}

FeatureModel& Regression::featureModel()
{
	return featureModel_;
}

Regression::Regression(const Experiment& expirement)
	: INode("Regression", "")
	, experiment_(expirement)
	, featureModel_(experiment_.getDataset())
	, linearRegressionModel_(featureModel_)
{

}

LinearRegressionModel& Regression::linearRegressionModel()
{
	return linearRegressionModel_;
}


void Regression::addNewChild()
{
	return;
}

INode::TypeObject Regression::type() const
{
	return TypeObject::Regression;
}
