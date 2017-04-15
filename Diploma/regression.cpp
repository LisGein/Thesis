#include "regression.h"
#include <mlpack/methods/linear_regression/linear_regression.hpp>

using namespace mlpack::regression;

arma::mat data; // The dataset itself.
arma::vec responses; // The responses, one row for each row in data.

// Regress.
LinearRegression lr(data,responses);

// Get the parameters, or coefficients.
arma::vec parameters = lr.Parameters();
