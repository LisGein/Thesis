#include "common/common.h"
#include "regression.h"
#include "experiment.h"
#include "featureWidget/featureGraphicsScene.h"


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

void Regression::removeAllChilds()
{
	return;
}

INode::TypeObject Regression::type() const
{
	return TypeObject::Regression;
}

void Regression::openRegression(boost::property_tree::ptree &regressions)
{
// TODO:openRegression
	namespace pt = boost::property_tree;
	for(auto &it:regressions)
	{
		pt::ptree feature = it.second;

		int cellFirst = -1;
		int cellSecond = -1;
		int i = 0;
		for (pt::ptree::value_type &cell : feature)
		{
			if (i ==0)
				cellFirst = cell.second.get_value<int>();
			else
				cellSecond = cell.second.get_value<int>();

			++i;
		}

		featureModel_.addFeature(FeatureModel::Feature(cellFirst, cellSecond));
	}
	linearRegressionModel_.update();
	featureModel_.updateData();
	featureModel_.getScene()->updateChecked();
}

void Regression::saveRegression(boost::property_tree::ptree &regressionTree)
{
	namespace pt = boost::property_tree;

	const std::set<FeatureModel::Feature>& feature = featureModel_.featureSet();

	for (auto &it : feature)
	{
		pt::ptree feature;

		pt::ptree cellFirst;
		cellFirst.put_value(it.first);
		feature.push_back(std::make_pair("", cellFirst));

		pt::ptree cellSecond;
		cellSecond.put_value(it.second);
		feature.push_back(std::make_pair("", cellSecond));

		regressionTree.push_back(std::make_pair("", feature));
	}

}
