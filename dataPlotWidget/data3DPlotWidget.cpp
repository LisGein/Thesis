#include "common/common.h"

#include "data3DPlotWidget.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QVBoxLayout>
#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/Q3DCamera>
#include <QtCore/qmath.h>
#include <QtGui/QScreen>
#include <QDebug>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>


using namespace QtDataVisualization;

Data3DPlotWidget::Data3DPlotWidget(QWidget* parent)
	: AbstractDataPlot(parent)
	, pointsGraph_(new Q3DScatter())
	, surfaceGraph_(new Q3DSurface())
	, pointsProxy_(new QScatterDataProxy(this))
	, surfaceProxy_(new QSurfaceDataProxy(this))
{
	QWidget *containerLeft = QWidget::createWindowContainer(pointsGraph_);


	QSize screenSize = pointsGraph_->screen()->size();
	containerLeft->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	containerLeft->setMaximumSize(screenSize);
	containerLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	containerLeft->setFocusPolicy(Qt::StrongFocus);
	layout()->addWidget(containerLeft);

	QWidget *containerRight = QWidget::createWindowContainer(surfaceGraph_);
	containerRight->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	containerRight->setMaximumSize(screenSize);
	containerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	containerRight->setFocusPolicy(Qt::StrongFocus);
	layout()->addWidget(containerRight);

	{
		pointsSeries_ = new QScatter3DSeries(pointsProxy_);
		pointsSeries_->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
		pointsSeries_->setMeshSmooth(true);


		pointsGraph_->axisX()->setTitle("X");
		pointsGraph_->axisY()->setTitle("Y");
		pointsGraph_->axisZ()->setTitle("Z");
		pointsGraph_->axisX()->setAutoAdjustRange(true);
		pointsGraph_->axisY()->setAutoAdjustRange(true);
		pointsGraph_->axisZ()->setAutoAdjustRange(true);
		pointsGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);
	}
	{
		surfaceSeries_ = new QSurface3DSeries(surfaceProxy_);
		surfaceSeries_->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
		surfaceSeries_->setFlatShadingEnabled(false);


		surfaceGraph_->setAxisX(new QValue3DAxis);
		surfaceGraph_->setAxisY(new QValue3DAxis);
		surfaceGraph_->setAxisZ(new QValue3DAxis);

		surfaceGraph_->axisX()->setTitle("X");
		surfaceGraph_->axisY()->setTitle("Y");
		surfaceGraph_->axisZ()->setTitle("Z");
		surfaceGraph_->axisX()->setRange(-1.1, 1.1);
		surfaceGraph_->axisZ()->setRange(-1.1, 1.1);
		surfaceGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);
		surfaceGraph_->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);


	}


}

Data3DPlotWidget::~Data3DPlotWidget()
{

}

void Data3DPlotWidget::clear()
{
	QList<QScatter3DSeries *> pointsSeries = pointsGraph_->seriesList();
	for (auto &it :pointsSeries)
		pointsGraph_->removeSeries(it);

	QList<QSurface3DSeries *> series = surfaceGraph_->seriesList();
	for (auto &it :series)
		surfaceGraph_->removeSeries(it);
}

void Data3DPlotWidget::updateChart(const arma::mat &data, const arma::vec &resp)
{
	if (axisZCombo_->box->currentIndex() == -1)
		return;

	int x = axisXCombo_->box->currentData().toInt();
	arma::vec xColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(x, 0));

	int z = axisZCombo_->box->currentData().toInt();
	arma::vec zColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(z, 0));


	updateDataChart(data, resp, xColumn, zColumn);
	updateRegressionChart(data, xColumn, zColumn);

	setGradient();
}

void Data3DPlotWidget::updateDataChart(const arma::mat& data, const arma::vec& resp, const arma::vec &xColumn, const arma::vec &zColumn)
{
	if (pointsGraph_->seriesList().empty())
		pointsGraph_->addSeries(pointsSeries_);

	QScatterDataArray *pointsArray = new QScatterDataArray;
	pointsArray->resize(data.n_rows);
	QScatterDataItem *ptrToDataArray = &pointsArray->first();

	for (size_t i = 0; i < data.n_rows; ++i)
	{

		QVector3D v(xColumn.at(i), resp.at(i), zColumn.at(i));//y axis looks top
		ptrToDataArray->setPosition(v);
		ptrToDataArray++;
	}

	pointsProxy_->resetArray(pointsArray);
}

void Data3DPlotWidget::updateRegressionChart(const arma::mat &data, const arma::vec &xColumn, const arma::vec &zColumn)
{
	if (surfaceGraph_->seriesList().empty())
		surfaceGraph_->addSeries(surfaceSeries_);

	QSurfaceDataArray *surfaceArray = new QSurfaceDataArray;

	auto xBounds = bounds(data, xColumn);
	auto zBounds = bounds(data, zColumn);

	surfaceGraph_->axisX()->setRange(xBounds.first - 0.1, xBounds.second + 0.1);
	surfaceGraph_->axisZ()->setRange(zBounds.first - 0.1, zBounds.second + 0.1);

	float stepX = (xBounds.second - xBounds.first) / GRID_SIZE;
	float stepZ = (zBounds.second - zBounds.first) / GRID_SIZE;

	double x = xBounds.first;



	std::map<int, double> raw;
	std::pair<int, int> ids(0, 1);

	for (const auto& key : axisIds_)
	{
		raw.emplace(key, 0.0);
		if (key == axisXCombo_->box->currentData().toInt())
			ids.first = key;
		if (key == axisZCombo_->box->currentData().toInt())
			ids.second = key;
	}

	while (x < xBounds.second)
	{
		QSurfaceDataRow *newRow = new QSurfaceDataRow;

		double z = zBounds.first;

		std::map<int, double> rawFeatures = raw;

		{
			auto iterX = rawFeatures.find(ids.first);
			if (iterX != rawFeatures.end())
				iterX->second = x;
		}

		while (z < zBounds.second)
		{
			{
				auto iterZ = rawFeatures.find(ids.second);
				if (iterZ != rawFeatures.end())
					iterZ->second = z;
			}

			arma::vec res = linearRegression_->getFeatureModel().getFinalFeaturesValue(rawFeatures);
			double predict = linearRegression_->predict(res);

			QVector3D v(x, predict, z);
			(*newRow).append(QSurfaceDataItem(v));
			z += stepZ;

		}
		surfaceArray->append(newRow);

		x += stepX;
	}
	surfaceProxy_->resetArray(surfaceArray);
}

void Data3DPlotWidget::setGradient()
{
	QLinearGradient gr;
	gr.setColorAt(0.0, Qt::black);
	gr.setColorAt(0.33, Qt::blue);
	gr.setColorAt(0.67, Qt::red);
	gr.setColorAt(1.0, Qt::yellow);

	surfaceSeries_->setBaseGradient(gr);
	surfaceSeries_->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
