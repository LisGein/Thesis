#include "plotController.h"

#include "dataPlotWidget/dataPlotWidget.h"
#include "dataPlotWidget/data3DPlotWidget.h"

#include "featureWidget/featureGraphicsScene.h"

#include "documentTree/regression.h"

#include <QVBoxLayout>
#include <QAction>
#include <QPushButton>


PlotController::PlotController(QWidget *parent)
	: QWidget(parent)
	, dataPlotWidget_(new DataPlotWidget(this))
	, data3DPlotWidget_(new Data3DPlotWidget(this))
{
	dataPlotWidget_->hide();

	setLayout(new QVBoxLayout(this));

	layout()->addWidget(dataPlotWidget_);
	layout()->addWidget(data3DPlotWidget_);

	QPushButton* act = new QPushButton(QObject::tr("Change graphic"), this);
	layout()->addWidget(act);
	QObject::connect(act, SIGNAL(released()), this, SLOT(changeGraphic()));
}

PlotController::~PlotController()
{

}

void PlotController::setRegression(Regression *regression)
{
	regression->featureModel().updateData();
	dataPlotWidget_->setRegression(&(regression->linearRegressionModel()));
	data3DPlotWidget_->setRegression(&(regression->linearRegressionModel()));
	QObject::connect(regression->featureModel().getScene(), SIGNAL(updatedRegression()), dataPlotWidget_, SLOT(updateRegression()));
	QObject::connect(regression->featureModel().getScene(), SIGNAL(updatedRegression()), data3DPlotWidget_, SLOT(updateRegression()));
}

void PlotController::setAxisNames(const std::set<int> &names)
{
	dataPlotWidget_->setAxisNames(names);
	data3DPlotWidget_->setAxisNames(names);
}

void PlotController::changeGraphic()
{
	if (dataPlotWidget_->isHidden())
	{
		data3DPlotWidget_->hide();
		dataPlotWidget_->show();
	}
	else
	{
		dataPlotWidget_->hide();
		data3DPlotWidget_->show();
	}
}
