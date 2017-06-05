#pragma once

#include <memory>

#include "abstractDataPlot.h"

#include "mlpack/core.hpp"

#include <QMap>


class LinearRegressionModel;
namespace QtDataVisualization {
class Q3DSurface;
class QSurfaceDataProxy;
class QSurface3DSeries;
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
	virtual void updateChart(const arma::mat& data, const arma::vec&) override;
	void setGradient();


	QtDataVisualization::Q3DSurface *surfaceGraph_;
	QtDataVisualization::QSurfaceDataProxy *surfaceProxy_;
	QtDataVisualization::QSurface3DSeries *surfaceSeries_;

	QMap<int, QString> axisByName_;
};
