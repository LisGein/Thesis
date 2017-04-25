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

	const Experiment& experiment_;
	FeatureModel featureModel_;
	LinearRegressionModel linearRegressionModel_;

};
