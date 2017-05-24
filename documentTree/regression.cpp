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
	, featureModel_(experiment_)
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

void Regression::openRegression(boost::property_tree::ptree &inventoryTree)
{

}

void Regression::saveRegression(boost::property_tree::ptree &inventoryTree)
{
    namespace pt = boost::property_tree;

    const std::set<FeatureModel::Feature>& feature = featureModel_.featureSet();

    for (auto &it : feature)
    {
        pt::ptree cellSecond;
        cellSecond.put_value(it.second);

        inventoryTree.push_back(std::make_pair(boost::lexical_cast<std::string>(it.first), cellSecond));
    }

}
