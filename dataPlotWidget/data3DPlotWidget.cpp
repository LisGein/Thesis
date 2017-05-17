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
        pointsGraph_->axisX()->setAutoAdjustRange(true);
        pointsGraph_->axisY()->setAutoAdjustRange(true);
        pointsGraph_->axisZ()->setAutoAdjustRange(true);
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

        surfaceGraph_->axisX()->setTitle("X");
        surfaceGraph_->axisY()->setTitle("Y");
        surfaceGraph_->axisZ()->setTitle("Z");
        surfaceGraph_->axisX()->setRange(-1.1, 1.1);
        surfaceGraph_->axisZ()->setRange(-1.1, 1.1);
        surfaceGraph_->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetRightHigh);
        surfaceGraph_->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);

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


    updateDataChart(data, resp);
    updateRegressionChart(data);
}

void Data3DPlotWidget::updateDataChart(const arma::mat& data, const arma::vec& resp)
{
    qDebug() << "x, y, z";
    QScatterDataArray *pointsArray = new QScatterDataArray;
    pointsArray->resize(data.n_rows);
    QScatterDataItem *ptrToDataArray = &pointsArray->first();


    for (size_t i = 0; i < data.n_rows; ++i)
    {

        QVector3D v(data.at(i, 0), resp.at(i), data.at(i, 1));//y axis looks top
        qDebug() << v;
        ptrToDataArray->setPosition(v);
        ptrToDataArray++;
    }

    pointsProxy_->resetArray(pointsArray);
}

void Data3DPlotWidget::updateRegressionChart(const auto& data)
{
    QSurfaceDataArray *surfaceArray = new QSurfaceDataArray;


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

    surfaceGraph_->axisX()->setRange(xBounds.first - 0.1, xBounds.second + 0.1);
    surfaceGraph_->axisZ()->setRange(zBounds.first - 0.1, zBounds.second + 0.1);
    qDebug() << "x, y, z predict";
    float stepX = (xBounds.second - xBounds.first) / (2*(data.n_rows - 1));
    float stepZ = (zBounds.second - zBounds.first) / (2*(data.n_rows - 1));

    double x = xBounds.first - stepX;
    do
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow;

        x += stepX;

        double z = zBounds.first - stepZ;

        size_t j = 0;
        do
        {
            z += stepZ;
            arma::vec p = arma::zeros(data.n_cols);
            p[0] = x;
            p[1] = z;
            double predict = linearRegression_->predict(p);

            QVector3D v(x, predict, z);
            qDebug() << v;
            (*newRow).append(QSurfaceDataItem(v));
            ++j;
        } while (z < zBounds.second);

        surfaceArray->append(newRow);

    } while (x < xBounds.second);
    surfaceProxy_->resetArray(surfaceArray);
}
