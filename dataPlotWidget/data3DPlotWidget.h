#pragma once

#include <memory>

#include "abstractDataPlot.h"

#include "mlpack/core.hpp"


class LinearRegressionModel;
namespace QtDataVisualization {
class Q3DScatter;
class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;
class QScatterDataProxy;
class QScatter3DSeries;
}
class QComboBox;

class Data3DPlotWidget : public AbstractDataPlot
{
    Q_OBJECT
public:
    Data3DPlotWidget(QWidget *parent = nullptr);
    virtual ~Data3DPlotWidget();

    virtual void setRegression(LinearRegressionModel* linearRegression) override;
    virtual void setAxisNames(const std::vector<std::string> &names) override;

private slots:
    void changeX(const QString &text);
    void changeZ(const QString &text);

private:
    void updateChart();
    void updateDataChart(const arma::mat& data, const arma::vec& resp, int axisX, int axisZ);
    void updateRegressionChart(const auto& data, int axisX, int axisZ);

    QtDataVisualization::Q3DScatter *pointsGraph_;
    QtDataVisualization::Q3DSurface *surfaceGraph_;

    QtDataVisualization::QScatterDataProxy *pointsProxy_;
    QtDataVisualization::QSurfaceDataProxy *surfaceProxy_;

    QtDataVisualization::QScatter3DSeries *pointsSeries_;
    QtDataVisualization::QSurface3DSeries *surfaceSeries_;

    QComboBox* axisXCombo_;
    QComboBox* axisZCombo_;
};
