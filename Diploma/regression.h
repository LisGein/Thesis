#pragma once
#include <string>
#include <vector>

class FeatureModel;

class LinearRegressionModel
{
public:
	LinearRegressionModel(const FeatureModel& featureModel);
	~LinearRegressionModel();

	void update();

	const std::vector<double>& getParams();
private:
	const FeatureModel& featureModel_;
	std::vector<double> parameters_;
};
