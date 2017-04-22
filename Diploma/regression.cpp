#include "common/common.h"
#include "regression.h"
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
	LinearRegression lr(regressors.t(), responses, 0, false);

	// Get the parameters, or coefficients.
	arma::vec parameters = lr.Parameters();
	parameters.print("Params:");

	parameters_.assign(parameters.begin(), parameters.end());
}

const std::vector<double>& LinearRegressionModel::getParams()
{
	return parameters_;
}
