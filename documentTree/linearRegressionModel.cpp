#include "common/common.h"
#include "linearRegressionModel.h"
#include "featureModel.h"

#include <mlpack/methods/linear_regression/linear_regression.hpp>


LinearRegressionModel::LinearRegressionModel(const FeatureModel& featureModel)
	: featureModel_(featureModel)
{
}

LinearRegressionModel::~LinearRegressionModel()
{

}

void LinearRegressionModel::update()
{
	using namespace mlpack::regression;

	arma::mat regressors = featureModel_.data();
	arma::vec responses = featureModel_.responses();

	regressors.print("Input data:");
	responses.print("Responses:");

	// Regress.
	linearRegression_ = std::make_unique<LinearRegression>(regressors.t(), responses, 0, false);

	// Get the parameters, or coefficients.
	arma::vec parameters = linearRegression_->Parameters();
	parameters.print("Params:");
}

const FeatureModel& LinearRegressionModel::getFeatureModel() const
{
	return featureModel_;
}

const arma::vec& LinearRegressionModel::getParams()
{
	return linearRegression_->Parameters();
}

double LinearRegressionModel::predict(const arma::vec& point) const
{
	arma::vec res;
	linearRegression_->Predict(arma::mat(point), res);
	return res(0);
}

