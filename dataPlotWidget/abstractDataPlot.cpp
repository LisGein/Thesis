#include "abstractDataPlot.h"

AbstractDataPlot::AbstractDataPlot(QWidget* parent)
	: QWidget(parent)
	, linearRegression_(nullptr)
{

}

AbstractDataPlot::~AbstractDataPlot()
{

}

void AbstractDataPlot::setRegression(LinearRegressionModel* linearRegression)
{
	linearRegression_ = linearRegression;
	updateChart();
}

void AbstractDataPlot::updateRegression()
{
	updateChart();
}
