#pragma once

#include <memory>

#include "abstractDataPlot.h"


class LinearRegressionModel;

namespace QtCharts{ class QChartView; }

class DataPlotWidget : public AbstractDataPlot
{
	Q_OBJECT
public:
	DataPlotWidget(QWidget *parent = nullptr);
	virtual ~DataPlotWidget();
	virtual void clear();

private:
	virtual void updateChart(const arma::mat& data, const arma::vec& resp) override;

private:
	std::unique_ptr<QtCharts::QChartView> chartView_;
};
