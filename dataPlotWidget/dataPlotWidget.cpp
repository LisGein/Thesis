#include "common/common.h"
#include "dataPlotWidget.h"
#include "chartSettings.h"
#include "ui_chartSettings.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QLineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>
#include <QPainter>
#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSplitter>

#include <algorithm>


DataPlotWidget::DataPlotWidget(QWidget* parent)
	: AbstractDataPlot(parent)
	, chartView_(std::make_unique<QtCharts::QChartView>())
{
	splitter_->addWidget(chartView_.get());
	chartSettings_->hideYBox();
}

DataPlotWidget::~DataPlotWidget()
{

}

void DataPlotWidget::clear()
{

}

void DataPlotWidget::updateChart(const arma::mat &data, const arma::vec &resp)
{

	using namespace QtCharts;

	QScatterSeries *pointsSeries = new QScatterSeries();
	pointsSeries->setName("points");
	pointsSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	pointsSeries->setMarkerSize(10.0);

	QLineSeries *regressionSeries = new QLineSeries();
	regressionSeries->setName("regression");


	// x in tearms of plot widget
	int xAxisColumn = chartSettings_->ui->boxX->currentData().toInt();
	arma::vec xColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(xAxisColumn, 0));


	auto xBounds = bounds(data, xColumn);

	for (size_t i = 0; i < data.n_rows; ++i)
		pointsSeries->append(xColumn.at(i), resp.at(i));


	float stepX = (xBounds.second - xBounds.first) / GRID_SIZE;

	double x = xBounds.first;

	std::map<int, double> rawFeatures;
	int idX = 0;

	int id = chartSettings_->ui->otherAxis->currentData().toInt();
	for (const auto& key : axisIds_)
	{
		if (key.first == id)
		{
			double n = chartSettings_->ui->doubleSpinBox->value();
			rawFeatures.emplace(key.first, n);
		}
		else
			rawFeatures.emplace(key.first, 0.0);

		if (key.first == chartSettings_->ui->boxX->currentData().toInt())
			idX = key.first;
	}

	while (x < xBounds.second)
	{
		auto iterX = rawFeatures.find(idX);
		if (iterX != rawFeatures.end())
			iterX->second = x;

		arma::vec res = linearRegression_->getFeatureModel().getFinalFeaturesValue(rawFeatures);
		double predict = linearRegression_->predict(res);
		regressionSeries->append(x, predict);
		x += stepX;
	}

	QChart *chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(pointsSeries);
	chart->addSeries(regressionSeries);
	chart->createDefaultAxes();
	chart->setTitle("Simple line chart example");

	chartView_->setRenderHint(QPainter::Antialiasing);
	chartView_->setChart(chart);

}















