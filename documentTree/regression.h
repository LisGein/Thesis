#pragma once

#include "iNode.h"
#include "linearRegressionModel.h"
#include "featureModel.h"


class Experiment;

class Regression : public INode
{
public:
	Regression(const Experiment& expirement);
	virtual ~Regression() = default;

	LinearRegressionModel& linearRegressionModel();
	FeatureModel& featureModel();
	// INode interface
public:
	virtual int childCount() const override;
	virtual INode* child(int id) override;
	virtual const INode* parentItem() const override;
	virtual void addNewChild() override;
	virtual void removeAllChilds() override;
	virtual TypeObject type() const override;

	virtual void openRegression(boost::property_tree::ptree &regressions) override;
	virtual void saveRegression(boost::property_tree::ptree &regressionTree) override;

private:
	const Experiment& experiment_;
	FeatureModel featureModel_;
	LinearRegressionModel linearRegressionModel_;

};
