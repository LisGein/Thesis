#include "regression.h"
#include <mlpack/methods/linear_regression/linear_regression.hpp>



void calcParams(const std::string& i_data)
{
	using namespace mlpack::regression;

	arma::mat regressors;
	auto sstr = std::istringstream(i_data);
	std::string header;
	std::getline(sstr, header);
	regressors.load(sstr, arma::csv_ascii); // The dataset itself.
	arma::vec responses = regressors.col(regressors.n_cols - 1);
	regressors.shed_col(regressors.n_cols - 1);

	regressors.print("Input data:");
	responses.print("Responses:");

	// Regress.
	LinearRegression lr(regressors.t(), responses);

	// Get the parameters, or coefficients.
	arma::vec parameters = lr.Parameters();
	parameters.print("Params:");
}
