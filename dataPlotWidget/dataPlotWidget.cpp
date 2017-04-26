#include "common/common.h"
#include "dataPlotWidget.h"
#include "ui_dataPlotWidget.h"

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

#include <algorithm>

DataPlotWidget::DataPlotWidget(QWidget* parent)
	: QWidget(parent)
	, linearRegression_(nullptr)
	, chartView_(std::make_unique<QtCharts::QChartView>())
	, ui_(std::make_unique<Ui::DataPlotWidget>())
{
	ui_->setupUi(this);
	ui_->chartPlace->layout()->addWidget(chartView_.get());


}

DataPlotWidget::~DataPlotWidget()
{

}

void DataPlotWidget::setRegression(LinearRegressionModel* linearRegression)
{
	linearRegression_ = linearRegression;
	updateChart();

	auto features = linearRegression_->getFeatureModel().getRawFeatures();
	for (const auto& feature : features)
	{
		ui_->axisCombo->addItem(to_qt(feature));
	}
	connect(ui_->plotBtn, SIGNAL(clicked()), this, SLOT(onPlotClicked()));
}

void DataPlotWidget::updateRegression()
{
	updateChart();
}

void DataPlotWidget::updateChart()
{
	if (linearRegression_)
	{
		using namespace QtCharts;

		QScatterSeries *series0 = new QScatterSeries();
		series0->setName("scatter1");
		series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
		series0->setMarkerSize(10.0);

		QLineSeries *series1 = new QLineSeries();
		series1->setName("scatter2");

		auto data = linearRegression_->getFeatureModel().data();
		auto resp = linearRegression_->getFeatureModel().responses();


		if (data.n_cols == 0 || data.n_rows == 0)
			return;

		// x in tearms of plot widget
		int xAxisColumn = data.n_cols - 1; //TODO: !!hardcode!! set with gui

		auto xBoundsIds = std::make_pair(0, 0);

		for (size_t i = 0; i < data.n_rows; ++i)
		{
			auto x = data.at(i, xAxisColumn);
			series0->append(x, resp.at(i));

			if (data.at(i, xAxisColumn) < data.at(xBoundsIds.first, xAxisColumn))
				xBoundsIds.first = i;
			if (data.at(i, xAxisColumn) > data.at(xBoundsIds.second, xAxisColumn))
				xBoundsIds.second = i;
		}

		double predictOnMin = linearRegression_->predict(data.row(xBoundsIds.first).t());
		series1->append(data.at(xBoundsIds.first, xAxisColumn), predictOnMin);

		double predictOnMax = linearRegression_->predict(data.row(xBoundsIds.second).t());
		series1->append(data.at(xBoundsIds.second, xAxisColumn), predictOnMax);


		QChart *chart = new QChart();
		chart->legend()->hide();
		chart->addSeries(series0);
		chart->addSeries(series1);
		chart->createDefaultAxes();
		chart->setTitle("Simple line chart example");

		chartView_->setRenderHint(QPainter::Antialiasing);
		chartView_->setChart(chart);
	}
}

void DataPlotWidget::onPlotClicked()
{
	updateChart();
}
