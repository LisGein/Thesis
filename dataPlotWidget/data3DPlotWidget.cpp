#include "common/common.h"

#include "data3DPlotWidget.h"
#include "chartSettings.h"
#include "ui_chartSettings.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QVBoxLayout>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/Q3DCamera>
#include <QtCore/qmath.h>
#include <QtGui/QScreen>
#include <QDebug>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QSplitter>

#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/QScatterDataProxy>


using namespace QtDataVisualization;

QValue3DAxis *axis(QObject *parent, const QString &name)
{
	QValue3DAxis *axis = new QValue3DAxis(parent);
	axis->setTitle(name);
	axis->setTitleVisible(true);

	axis->setLabelAutoRotation(30.0f);
	return axis;
}

Data3DPlotWidget::Data3DPlotWidget(QWidget* parent)
	: AbstractDataPlot(parent)
	, surfaceGraph_(new Q3DSurface())
	, surfaceProxy_(new QSurfaceDataProxy(this))
	, surfaceSeries_(new QSurface3DSeries(surfaceProxy_))
	, pointsGraph_(new Q3DScatter())
	, pointsProxy_(new QScatterDataProxy(this))
	, pointsSeries_(new QScatter3DSeries(pointsProxy_))
{
	QSize screenSize = surfaceGraph_->screen()->size();


	//	QWidget *containerR = QWidget::createWindowContainer(pointsGraph_);
	//	containerR->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	//	containerR->setMaximumSize(screenSize);
	//	containerR->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//	containerR->setFocusPolicy(Qt::StrongFocus);
	//	splitter_->addWidget(containerR);

	QWidget *container = QWidget::createWindowContainer(surfaceGraph_);
	container->setMinimumSize(QSize(screenSize.width() / 4, screenSize.height() / 2));
	container->setMaximumSize(screenSize);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	container->setFocusPolicy(Qt::StrongFocus);
	splitter_->addWidget(container);

	{
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

	surfaceSeries_->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
	surfaceSeries_->setFlatShadingEnabled(false);




	surfaceGraph_->setAxisX(axis(surfaceGraph_, "X"));
	surfaceGraph_->setAxisY(axis(surfaceGraph_, "Y"));
	surfaceGraph_->setAxisZ(axis(surfaceGraph_, "Z"));


	surfaceGraph_->axisX()->setRange(-1.1, 1.1);
	surfaceGraph_->axisZ()->setRange(-1.1, 1.1);
	surfaceGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);
	surfaceGraph_->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
}

Data3DPlotWidget::~Data3DPlotWidget()
{

}

void Data3DPlotWidget::clear()
{
	QList<QSurface3DSeries *> series = surfaceGraph_->seriesList();
	for (auto &it :series)
		surfaceGraph_->removeSeries(it);

	QList<QScatter3DSeries *> pointsSeries = pointsGraph_->seriesList();
	for (auto &it :pointsSeries)
		pointsGraph_->removeSeries(it);
}

void Data3DPlotWidget::updateChart(const arma::mat &data, const arma::vec &resp)
{
	if (chartSettings_->ui->boxY->currentIndex() == -1)
		return;

	int xName = chartSettings_->ui->boxX->currentData().toInt();
	arma::vec xColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(xName, 0));

	int zName = chartSettings_->ui->boxY->currentData().toInt();
	arma::vec zColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(zName, 0));


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




	surfaceGraph_->setAxisX(axis(surfaceGraph_, chartSettings_->ui->boxX->currentText()));
	surfaceGraph_->setAxisY(axis(surfaceGraph_, QString::fromStdString(linearRegression_->getFeatureModel().getResponseName())));
	surfaceGraph_->setAxisZ(axis(surfaceGraph_, chartSettings_->ui->boxY->currentText()));

	if (surfaceGraph_->seriesList().empty())
		surfaceGraph_->addSeries(surfaceSeries_);

	QSurfaceDataArray *surfaceArray = new QSurfaceDataArray;

	auto xBounds = bounds(data, xColumn);
	auto zBounds = bounds(data, zColumn);


	float stepX = (xBounds.second - xBounds.first) / GRID_SIZE;
	float stepZ = (zBounds.second - zBounds.first) / GRID_SIZE;

	double x = xBounds.first;

	std::map<int, double> raw;
	std::pair<int, int> ids(0, 1);


	int id = chartSettings_->ui->otherAxis->currentData().toInt();
	for (const auto& key : axisIds_)
	{
		if (key.first == id)
		{
			double n = chartSettings_->ui->doubleSpinBox->value();
			raw.emplace(key.first, n);
		}
		else
			raw.emplace(key.first, 0.0);

		if (key.first == chartSettings_->ui->boxX->currentData().toInt())
			ids.first = key.first;
		if (key.first == chartSettings_->ui->boxY->currentData().toInt())
			ids.second = key.first;
	}

	double min = (xBounds.first < zBounds.first) ? xBounds.first : zBounds.first;
	double max = (xBounds.second < zBounds.second) ? zBounds.second : xBounds.second;


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
			min = (min < predict) ? min : predict;
			max = (max > predict) ? max : predict;

			QVector3D v(x, predict, z);
			(*newRow).append(QSurfaceDataItem(v));
			z += stepZ;

		}
		surfaceArray->append(newRow);

		x += stepX;
	}

	surfaceGraph_->axisX()->setRange(min - 0.1, max + 0.1);
	surfaceGraph_->axisZ()->setRange(min - 0.1, max + 0.1);
	surfaceGraph_->axisY()->setRange(min - 0.1, max + 0.1);

	surfaceProxy_->resetArray(surfaceArray);

	setGradient();
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
