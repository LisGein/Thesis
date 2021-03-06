#pragma once
#include <string>
#include <vector>
#include <memory>

#include <armadillo>


namespace mlpack
{
namespace regression
{
class LinearRegression;
}
}

class FeatureModel;

class LinearRegressionModel
{
public:
	LinearRegressionModel(const FeatureModel& featureModel);
	virtual ~LinearRegressionModel();

	void update();
	const FeatureModel& getFeatureModel() const;

	const arma::vec& getParams();
	double predict(const arma::vec& point) const;

private:
	const FeatureModel& featureModel_;
	std::unique_ptr<mlpack::regression::LinearRegression> linearRegression_;
};
