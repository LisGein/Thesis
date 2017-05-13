#pragma once

#include <memory>

#include "abstractDataPlot.h"


class LinearRegressionModel;
namespace QtDataVisualization {
class Q3DScatter;
class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;
class QScatterDataProxy;
class QScatter3DSeries;
}

class Data3DPlotWidget : public AbstractDataPlot
{
	Q_OBJECT
public:
	Data3DPlotWidget(QWidget *parent = nullptr);
	virtual ~Data3DPlotWidget();

	virtual void setRegression(LinearRegressionModel* linearRegression);


private:
	void updateChart();

	QtDataVisualization::Q3DScatter *pointsGraph_;
	QtDataVisualization::Q3DSurface *surfaceGraph_;

	QtDataVisualization::QScatterDataProxy *pointsProxy_;
	QtDataVisualization::QSurfaceDataProxy *surfaceProxy_;

	QtDataVisualization::QScatter3DSeries *pointsSeries_;
	QtDataVisualization::QSurface3DSeries *surfaceSeries_;

};
