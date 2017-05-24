#pragma once

#include "iNode.h"
#include "linearRegressionModel.h"
#include "featureModel.h"

class Experiment;

class Regression : public INode
{
public:
	Regression(const Experiment& expirement);
	LinearRegressionModel& linearRegressionModel();
	FeatureModel& featureModel();
	// INode interface
public:
	int childCount() const override;
	INode* child(int id) override;
	const INode* parentItem() const override;
	void addNewChild() override;
	virtual TypeObject type() const override;


    virtual void openRegression(boost::property_tree::ptree &inventoryTree) override;
    virtual void saveRegression(boost::property_tree::ptree &inventoryTree) override;

	const Experiment& experiment_;
	FeatureModel featureModel_;
	LinearRegressionModel linearRegressionModel_;

};
