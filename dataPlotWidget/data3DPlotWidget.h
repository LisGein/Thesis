#pragma once

#include <memory>

#include "abstractDataPlot.h"

#include "mlpack/core.hpp"

#include <QMap>


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
	virtual void clear();


private:
	virtual void updateChart(const arma::mat& data, const arma::vec& resp) override;

	void updateDataChart(const arma::mat& data, const arma::vec& resp, const arma::vec &xColumn, const arma::vec &zColumn);
	void updateRegressionChart(const arma::mat& data, const arma::vec &xColumn, const arma::vec &zColumn);


	void setGradient();

	QtDataVisualization::Q3DScatter *pointsGraph_;
	QtDataVisualization::Q3DSurface *surfaceGraph_;

	QtDataVisualization::QScatterDataProxy *pointsProxy_;
	QtDataVisualization::QSurfaceDataProxy *surfaceProxy_;

	QtDataVisualization::QScatter3DSeries *pointsSeries_;
	QtDataVisualization::QSurface3DSeries *surfaceSeries_;


	QMap<int, QString> axisByName_;
};
