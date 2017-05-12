#pragma once

#include <memory>

#include "abstractDataPlot.h"


class LinearRegressionModel;
namespace QtDataVisualization {
class Q3DScatter;

class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;
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

	QtDataVisualization::Q3DScatter *graphData_;


	QtDataVisualization::Q3DSurface *graphRegression;
	QtDataVisualization::QSurfaceDataProxy *m_sqrtSinProxy;
	QtDataVisualization::QSurface3DSeries *m_heightMapSeries;
	QtDataVisualization::QSurface3DSeries *m_sqrtSinSeries;

};
