#include "common/common.h"
#include "dataPlotWidget.h"

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

#include <algorithm>


DataPlotWidget::DataPlotWidget(QWidget* parent)
    : AbstractDataPlot(parent)
    , chartView_(std::make_unique<QtCharts::QChartView>())
{
    layout()->addWidget(chartView_.get());
    axisZCombo_->hide();
}

DataPlotWidget::~DataPlotWidget()
{

}

void DataPlotWidget::updateChart(const arma::mat &data, const arma::vec &resp)
{

    using namespace QtCharts;

    QScatterSeries *series0 = new QScatterSeries();
    series0->setName("scatter1");
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(10.0);

    QLineSeries *series1 = new QLineSeries();
    series1->setName("scatter2");


    // x in tearms of plot widget
    int xAxisColumn = axisXCombo_->box->currentData().toInt();
    arma::vec xColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(xAxisColumn, 0));


    auto xBounds = bounds(data, xColumn);

    for (size_t i = 0; i < data.n_rows; ++i)
        series0->append(xColumn.at(i), resp.at(i));


    float stepX = (xBounds.second - xBounds.first) / GRID_SIZE;

    double x = xBounds.first;

    std::map<int, double> rawFeatures;
    int idX = 0;

    for (const auto& key : axisIds_)
    {
        rawFeatures.emplace(key, 0.0);
        if (key == axisXCombo_->box->currentData().toInt())
            idX = key;
    }

    while (x < xBounds.second)
    {
        auto iterX = rawFeatures.find(idX);
        if (iterX != rawFeatures.end())
            iterX->second = x;

        arma::vec res = linearRegression_->getFeatureModel().getFinalFeaturesValue(rawFeatures);
        double predict = linearRegression_->predict(res);
        series1->append(x, predict);
        x += stepX;
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series0);
    chart->addSeries(series1);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    chartView_->setRenderHint(QPainter::Antialiasing);
    chartView_->setChart(chart);

}















