#include "common/common.h"
#include "viewController.h"
#include "datasetWidget/datasetView.h"
#include "datasetWidget/datasetModel.h"
#include "dataPlotWidget/dataPlotWidget.h"
#include "featureWidget/featureGraphicsScene.h"
#include "documentTree/document.h"
#include "documentTree/dataset.h"
#include "documentTree/featureModel.h"
#include "documentTree/regression.h"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QLineEdit>
#include <QTreeView>

#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>


ViewController::ViewController(QWidget* parent)
	: QWidget(parent)
	, gview_(new QGraphicsView(this))
	, lineEdit_(new QLineEdit(this))
{
	setLayout(new QVBoxLayout(this));

	QWidget *w = new QWidget(this);
	w->setLayout(new QVBoxLayout(this));
	w->layout()->addWidget(gview_);
	w->layout()->addWidget(lineEdit_);
	layout()->addWidget(w);

	auto* dataPlotWidget = new DataPlotWidget(this);
	layout()->addWidget(dataPlotWidget);
}

ViewController::~ViewController()
{

}

void ViewController::setScene(FeatureGraphicsScene* scene)
{
	gview_->setScene(scene);
}



