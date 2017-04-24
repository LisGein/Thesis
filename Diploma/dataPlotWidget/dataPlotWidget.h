#pragma once

#include <QWidget>
#include <memory>

class LinearRegressionModel;

namespace QtCharts
{
	class QChartView;
}

namespace Ui
{
class DataPlotWidget;
}

class DataPlotWidget : public QWidget
{
	Q_OBJECT
public:
	DataPlotWidget(const LinearRegressionModel& linearRegression, QWidget *parent = nullptr);
	~DataPlotWidget();

private:
	void updateChart();

private slots:
	void onPlotClicked();


private:
	const LinearRegressionModel& linearRegression_;
	std::unique_ptr<QtCharts::QChartView> chartView_;

	std::unique_ptr<Ui::DataPlotWidget> ui_;
};
