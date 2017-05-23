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
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>


const int GRID_SIZE = 20;

using namespace QtDataVisualization;

Data3DPlotWidget::Data3DPlotWidget(QWidget* parent)
    : AbstractDataPlot(parent)
    , pointsGraph_(new Q3DScatter())
    , surfaceGraph_(new Q3DSurface())
    , surfaceProxy_(new QSurfaceDataProxy(this))
    , pointsProxy_(new QScatterDataProxy(this))
    , axisXCombo_(new QComboBox(this))
    , axisZCombo_(new QComboBox(this))
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


    QWidget* axisWidget = new QWidget(this);
    QGridLayout* lay = new QGridLayout(axisWidget);

    QLabel *labelX = new QLabel("Axis x:", axisWidget);
    lay->addWidget(labelX, 0, 0);
    lay->addWidget(axisXCombo_, 0, 1);
    QLabel *labelZ = new QLabel("Axis z:", axisWidget);
    lay->addWidget(labelZ, 1, 0);
    lay->addWidget(axisZCombo_, 1, 1);


    axisWidget->setLayout(lay);


    layout()->addWidget(axisWidget);

    connect(axisXCombo_, SIGNAL(currentTextChanged(const QString &)), this, SLOT(changeX(const QString &)));
    connect(axisZCombo_, SIGNAL(currentTextChanged(const QString &)), this, SLOT(changeZ(const QString &)));

}

Data3DPlotWidget::~Data3DPlotWidget()
{

}

void Data3DPlotWidget::setRegression(LinearRegressionModel* linearRegression)
{
    AbstractDataPlot::setRegression(linearRegression);
    updateChart();
}

void Data3DPlotWidget::setAxisNames(const std::set<int> &axisIds)
{
    AbstractDataPlot::setAxisNames(axisIds);
    axisXCombo_->clear();
    axisZCombo_->clear();

    const FeatureModel& model = linearRegression_->getFeatureModel();
    for (const auto& feature : axisIds_)
    {
        QString name = to_qt(model.idTofeatureName(feature));
        axisByName_.insert(feature, name);
        axisXCombo_->addItem(name, QVariant(feature));
        axisZCombo_->addItem(name, QVariant(feature));
    }
    axisZCombo_->setCurrentIndex(1);
}

void Data3DPlotWidget::changeX(const QString &)
{
    updateChart();
}

void Data3DPlotWidget::changeZ(const QString &)
{
    updateChart();
}

void Data3DPlotWidget::updateChart()
{

    auto data = linearRegression_->getFeatureModel().data();
    auto resp = linearRegression_->getFeatureModel().responses();

    if (data.n_cols < 2 || data.n_rows == 0)
        return;

    int x = axisXCombo_->currentData().toInt();
    if (x == -1)
        x = 0;


    int z = axisZCombo_->currentData().toInt();
    if (z == -1)
        z = 0;


    arma::vec xColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(x, 0));
    arma::vec zColumn = linearRegression_->getFeatureModel().columnAt(FeatureModel::Feature(z, 0));


    updateDataChart(data, resp, xColumn, zColumn);
    updateRegressionChart(data, xColumn, zColumn);
}

void Data3DPlotWidget::updateDataChart(const arma::mat& data, const arma::vec& resp, const arma::vec &xColumn, const arma::vec &zColumn)
{
    qDebug() << "x, y, z";
    QScatterDataArray *pointsArray = new QScatterDataArray;
    pointsArray->resize(data.n_rows);
    QScatterDataItem *ptrToDataArray = &pointsArray->first();

    for (size_t i = 0; i < data.n_rows; ++i)
    {

        QVector3D v(xColumn.at(i), resp.at(i), zColumn.at(i));//y axis looks top
        qDebug() << v;
        ptrToDataArray->setPosition(v);
        ptrToDataArray++;
    }

    pointsProxy_->resetArray(pointsArray);
}

void Data3DPlotWidget::updateRegressionChart(const arma::mat &data, const arma::vec &xColumn, const arma::vec &zColumn)
{
    QSurfaceDataArray *surfaceArray = new QSurfaceDataArray;


    std::pair<double, double> xBounds, zBounds;
    bool firstTime = true;

    for (size_t i = 0; i < data.n_rows; ++i)
    {
        auto x = xColumn.at(i);
        auto z = zColumn.at(i);

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
    float stepX = (xBounds.second - xBounds.first) / GRID_SIZE;
    float stepZ = (zBounds.second - zBounds.first) / GRID_SIZE;

    double x = xBounds.first - stepX;



    std::map<int, double> raw;
    int idX = -1;
    int idZ = -1;

//FIXME remove comparison with String
    for (QMap<int, QString>::iterator feature = axisByName_.begin(); feature != axisByName_.end(); ++feature)
    {
        int key = feature.key();
        raw.emplace(key, 0.0);
        if (feature.value() == axisXCombo_->currentText())
            idX = key;
        if (feature.value() == axisZCombo_->currentText())
            idZ = key;
    }

    do
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow;

        x += stepX;

        double z = zBounds.first - stepZ;

        size_t j = 0;
        std::map<int, double> rawFeatures = raw;

        auto iterX = rawFeatures.find(idX);
        if (iterX != rawFeatures.end())
            iterX->second = x;
        do
        {
            z += stepZ;


            auto iterZ = rawFeatures.find(idZ);
            if (iterZ != rawFeatures.end())
                iterZ->second = z;

            arma::vec res = linearRegression_->getFeatureModel().getFinalFeaturesValue(rawFeatures);
            double predict = linearRegression_->predict(res);

            QVector3D v(x, predict, z);
            qDebug() << v;
            (*newRow).append(QSurfaceDataItem(v));
            ++j;
        } while (z < zBounds.second);

        surfaceArray->append(newRow);

    } while (x < xBounds.second);
    surfaceProxy_->resetArray(surfaceArray);
}
