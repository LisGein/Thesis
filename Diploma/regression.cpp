#include "regression.h"
#include <mlpack/methods/linear_regression/linear_regression.hpp>



void calcParams(const std::vector<double>& i_data, const std::vector<double>& i_res, int column, int row)
{
	using namespace mlpack::regression;

	arma::mat regressors = (i_data); // The dataset itself.
	regressors.reshape(column, row);
	regressors = regressors.t();
	arma::vec responses = i_res;/*
	regressors.col(4);
	regressors.shed_col(4);*/

	regressors.print("Input data:");
	responses.print("Responses:");

	// Regress.
	LinearRegression lr(regressors.t(), responses, 0, false);

	// Get the parameters, or coefficients.
	arma::vec parameters = lr.Parameters();
	parameters.print("Params:");
}
