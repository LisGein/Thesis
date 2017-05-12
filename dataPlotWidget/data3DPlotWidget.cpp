#include "data3DPlotWidget.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QVBoxLayout>
#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/Q3DCamera>
#include <QtCore/qmath.h>
#include <QtGui/QScreen>


#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

Data3DPlotWidget::Data3DPlotWidget(QWidget* parent)
	: AbstractDataPlot(parent)
	, graphData_(new Q3DScatter())
	, graphRegression(new Q3DSurface())
{
	setLayout(new QHBoxLayout(this));

	QWidget *containerLeft = QWidget::createWindowContainer(graphData_);


	QSize screenSize = graphData_->screen()->size();
	containerLeft->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	containerLeft->setMaximumSize(screenSize);
	containerLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	containerLeft->setFocusPolicy(Qt::StrongFocus);

	layout()->addWidget(containerLeft);

	QWidget *containerRight = QWidget::createWindowContainer(graphRegression);
	containerRight->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	containerRight->setMaximumSize(screenSize);
	containerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	containerRight->setFocusPolicy(Qt::StrongFocus);
	layout()->addWidget(containerRight);




	{
		QScatterDataProxy *proxy = new QScatterDataProxy;
		QScatter3DSeries *series = new QScatter3DSeries(proxy);
		series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
		series->setMeshSmooth(true);
		graphData_->addSeries(series);


		graphData_->axisX()->setTitle("X");
		graphData_->axisY()->setTitle("Y");
		graphData_->axisZ()->setTitle("Z");
		graphData_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);

	}

	{


		graphRegression->setAxisX(new QValue3DAxis);
		graphRegression->setAxisY(new QValue3DAxis);
		graphRegression->setAxisZ(new QValue3DAxis);
		graphRegression->axisX()->setAutoAdjustRange(true);
		graphRegression->axisY()->setAutoAdjustRange(true);
		graphRegression->axisZ()->setAutoAdjustRange(true);
		graphRegression->axisX()->setTitle("X");
		graphRegression->axisY()->setTitle("Y");
		graphRegression->axisZ()->setTitle("Z");
		graphRegression->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);

		m_sqrtSinProxy = new QSurfaceDataProxy();
		m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);


		m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
		m_sqrtSinSeries->setFlatShadingEnabled(true);
		graphRegression->addSeries(m_sqrtSinSeries);/*

		//set green to red gradient

		{

			QLinearGradient gr;
			gr.setColorAt(-2.0, Qt::darkGreen);
			gr.setColorAt(0.5, Qt::yellow);
			gr.setColorAt(2.0, Qt::red);
			gr.setColorAt(4.0, Qt::darkRed);

			graphRegression->seriesList().at(0)->setBaseGradient(gr);
			graphRegression->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
		}*/

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

	if (data.n_cols == 0 || data.n_rows == 0)
		return;


	QScatterDataArray *dataArray = new QScatterDataArray;
	dataArray->resize(data.n_rows);
	QScatterDataItem *ptrToDataArray = &dataArray->first();


	for (size_t i = 0; i < data.n_rows; ++i)
	{
		ptrToDataArray->setPosition(QVector3D(data.at(i, 0), data.at(i, 1), resp.at(i)));
		ptrToDataArray++;
	}

	graphData_->seriesList().at(0)->dataProxy()->resetArray(dataArray);


	QSurfaceDataArray *regresssionArray = new QSurfaceDataArray;
	regresssionArray->reserve(data.size());


	auto xBoundsIds = std::make_pair(0.0, 0.0);
	auto yBoundsIds = std::make_pair(0.0, 0.0);

	for (size_t i = 0; i < data.n_rows; ++i)
	{
		auto x = data.at(i, 0);
		auto y = data.at(i, 1);

		if (i == 0)
		{
			xBoundsIds = std::make_pair(x, x);
			yBoundsIds = std::make_pair(y, y);
		}
		else
		{

			if (x < xBoundsIds.first)
				xBoundsIds.first = x;
			if (x > xBoundsIds.second)
				xBoundsIds.second = x;


			if (y < yBoundsIds.first)
				yBoundsIds.first = y;
			if (y > yBoundsIds.second)
				yBoundsIds.second = y;
		}
	}






	for (size_t i = 0; i < 10; ++i)
	{

		QSurfaceDataRow *newRow = new QSurfaceDataRow(10);

		double x = xBoundsIds.first + i *((xBoundsIds.second - xBoundsIds.first)/data.n_rows);
		for (int j = 0; j < 10; j++)
		{
			double y = yBoundsIds.first + j *((yBoundsIds.second - yBoundsIds.first)/data.n_rows);
			arma::vec p = arma::zeros(data.n_cols);
			p[0] = x;
			p[1] = y;
			double predict = linearRegression_->predict(p);
			//double predict = x * y;
			QVector3D v(x, y, predict);
			(*newRow)[0].setPosition(v);
		}

		*regresssionArray << newRow;

	}

	m_sqrtSinProxy->resetArray(regresssionArray);
}
