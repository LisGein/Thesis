#include "common/common.h"
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

void AbstractDataPlot::setAxisNames(const std::set<int> &axisIds)
{
    axisIds_ = axisIds;
}

void AbstractDataPlot::updateRegression()
{
	updateChart();
}
