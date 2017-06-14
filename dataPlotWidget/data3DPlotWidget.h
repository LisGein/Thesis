#pragma once

#include <memory>

#include "abstractDataPlot.h"

#include <armadillo>

#include <QMap>


class LinearRegressionModel;
namespace QtDataVisualization {
class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;

class Q3DScatter;
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
	virtual void clear();


private:
	virtual void updateChart(const arma::mat& data, const arma::vec&resp) override;
	void setGradient();


	QtDataVisualization::Q3DSurface *surfaceGraph_;
	QtDataVisualization::QSurfaceDataProxy *surfaceProxy_;
	QtDataVisualization::QSurface3DSeries *surfaceSeries_;

	QtDataVisualization::Q3DScatter *pointsGraph_;
	QtDataVisualization::QScatterDataProxy *pointsProxy_;
	QtDataVisualization::QScatter3DSeries *pointsSeries_;

	QMap<int, QString> axisByName_;
};
