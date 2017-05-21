#pragma once

#include <memory>

#include "abstractDataPlot.h"


class LinearRegressionModel;

namespace QtCharts{ class QChartView; }
namespace Ui{ class DataPlotWidget; }

class DataPlotWidget : public AbstractDataPlot
{
    Q_OBJECT
public:
    DataPlotWidget(QWidget *parent = nullptr);
    ~DataPlotWidget();

    virtual void setRegression(LinearRegressionModel* linearRegression) override;
    virtual void setAxisNames(const std::set<int> &axisIds) override;

private:
    virtual void updateChart();

private slots:
    void changeX(const QString &text);

private:
    std::unique_ptr<QtCharts::QChartView> chartView_;
    std::unique_ptr<Ui::DataPlotWidget> ui_;
};
