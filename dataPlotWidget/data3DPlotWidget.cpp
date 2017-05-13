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




using namespace QtDataVisualization;

Data3DPlotWidget::Data3DPlotWidget(QWidget* parent)
	: AbstractDataPlot(parent)
	, pointsGraph_(new Q3DScatter())
	, surfaceGraph_(new Q3DSurface())
{
	setLayout(new QHBoxLayout(this));

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
		pointsProxy_ = new QScatterDataProxy;
		pointsSeries_ = new QScatter3DSeries(pointsProxy_);
		pointsSeries_->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
		pointsSeries_->setMeshSmooth(true);
		pointsGraph_->addSeries(pointsSeries_);


		pointsGraph_->axisX()->setTitle("X");
		pointsGraph_->axisY()->setTitle("Y");
		pointsGraph_->axisZ()->setTitle("Z");
		pointsGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);
	}

	{
		surfaceProxy_ = new QSurfaceDataProxy();
		surfaceSeries_ = new QSurface3DSeries(surfaceProxy_);
		surfaceSeries_->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
		surfaceSeries_->setFlatShadingEnabled(false);

		surfaceGraph_->addSeries(surfaceSeries_);

		surfaceGraph_->setAxisX(new QValue3DAxis);
		surfaceGraph_->setAxisY(new QValue3DAxis);
		surfaceGraph_->setAxisZ(new QValue3DAxis);
		surfaceGraph_->axisX()->setAutoAdjustRange(true);
		surfaceGraph_->axisY()->setAutoAdjustRange(true);
		surfaceGraph_->axisZ()->setAutoAdjustRange(true);
		surfaceGraph_->axisX()->setTitle("X");
		surfaceGraph_->axisY()->setTitle("Y");
		surfaceGraph_->axisZ()->setTitle("Z");
		surfaceGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);

		//set green to red gradient

		{

			QLinearGradient gr;
			gr.setColorAt(-2.0, Qt::darkGreen);
			gr.setColorAt(0.5, Qt::yellow);
			gr.setColorAt(2.0, Qt::red);
			gr.setColorAt(4.0, Qt::darkRed);

			surfaceSeries_->setBaseGradient(gr);
			surfaceSeries_->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
		}

	}

}

Data3DPlotWidget::~Data3DPlotWidget()
{

}

void Data3DPlotWidget::setRegression(LinearRegressionModel* linearRegression)
{
	AbstractDataPlot::setRegression(linearRegression);
	updateChart();

}

void Data3DPlotWidget::updateChart()
{

	auto data = linearRegression_->getFeatureModel().data();
	auto resp = linearRegression_->getFeatureModel().responses();

	if (data.n_cols < 2 || data.n_rows == 0)
		return;


	QScatterDataArray *pointsArray = new QScatterDataArray;
	pointsArray->resize(data.n_rows);
	QScatterDataItem *ptrToDataArray = &pointsArray->first();


	for (size_t i = 0; i < data.n_rows; ++i)
	{
		ptrToDataArray->setPosition(QVector3D(data.at(i, 0), resp.at(i), data.at(i, 1))); //y axis looks top
		ptrToDataArray++;
	}

	pointsProxy_->resetArray(pointsArray);


	QSurfaceDataArray *surfaceArray = new QSurfaceDataArray;
	surfaceArray->reserve(10);


	std::pair<double, double> xBounds, zBounds;
	bool firstTime = true;

	for (size_t i = 0; i < data.n_rows; ++i)
	{
		auto x = data.at(i, 0);
		auto z = data.at(i, 1);

		if (firstTime)
		{
			xBounds = std::make_pair(x, x);
			zBounds = std::make_pair(z, z);
			firstTime = false;
		}
		else
		{

			if (x < xBounds.first)
				xBounds.first = x;
			if (x > xBounds.second)
				xBounds.second = x;


			if (z < zBounds.first)
				zBounds.first = z;
			if (z > zBounds.second)
				zBounds.second = z;
		}
	}


	float stepX = (xBounds.second - xBounds.first) / (data.n_rows - 1);
	float stepZ = (zBounds.second - zBounds.first) / (data.n_rows - 1);

	for (size_t i = 0; i < 10; ++i)
	{
		QSurfaceDataRow *newRow = new QSurfaceDataRow(10);
		double x = xBounds.first + i * stepX;

		for (int j = 0; j < 10; j++)
		{
			double z = zBounds.first + j * stepZ;

			arma::vec p = arma::zeros(data.n_cols);
			p[0] = x;
			p[1] = z;
			double predict = linearRegression_->predict(p);

			QVector3D v(x, predict, z);
			(*newRow)[j].setPosition(v);
		}

		*surfaceArray << newRow;

	}

	surfaceProxy_->resetArray(surfaceArray);
}
