#include "common/common.h"
#include "viewController.h"
#include "plotController.h"
#include "datasetWidget/datasetView.h"
#include "datasetWidget/datasetModel.h"
#include "dataPlotWidget/dataPlotWidget.h"
#include "dataPlotWidget/data3DPlotWidget.h"
#include "featureWidget/featureGraphicsScene.h"
#include "documentTree/document.h"
#include "documentTree/dataset.h"
#include "documentTree/featureModel.h"
#include "documentTree/regression.h"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QLineEdit>
#include <QTreeView>
#include <QDebug>
#include <QSplitter>

#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>


ViewController::ViewController(QWidget* parent)
    : QWidget(parent)
    , gview_(new QGraphicsView(this))
    , lineEdit_(new QLineEdit(this))
    , regression_(nullptr)
    , plotController_ (new PlotController(this))
{
    setLayout(new QVBoxLayout(this));

    QWidget *w = new QWidget(this);
	w->setLayout(new QVBoxLayout());
    w->layout()->addWidget(gview_);
    w->layout()->addWidget(lineEdit_);

    QSplitter *split = new QSplitter(this);
    split->setOrientation(Qt::Vertical);
    split->addWidget(w);
    split->addWidget(plotController_);
    split->setChildrenCollapsible(false);

    layout()->addWidget(split);

}

ViewController::~ViewController()
{

}

void ViewController::setRegression(Regression* regression)
{
    regression_ = regression;

    gview_->setScene(regression_->featureModel().getScene());
    QObject::connect(regression_->featureModel().getScene(), SIGNAL(deletedFromFormula(QPair<int, int>)), this, SLOT(deleteFromFormula(QPair<int, int>)));
    QObject::connect(regression_->featureModel().getScene(), SIGNAL(addedToFormula(QPair<int, int>)), this, SLOT(addToFormula(QPair<int, int>)));

    plotController_->setRegression(regression);
    updateRegression();
}

bool ViewController::containRegression() const
{
    return regression_;
}

void ViewController::update()
{
    regression_->featureModel().update();
    gview_->update();
}

void ViewController::deleteFromFormula(QPair<int, int> feature)
{
    regression_->featureModel().removeFeature(from_qt(feature));
    updateRegression();
}

void ViewController::addToFormula(QPair<int, int> feature)
{
    regression_->featureModel().addFeature(from_qt(feature));
    updateRegression();
}

void ViewController::updateRegression()
{
    regression_->linearRegressionModel().update();
    plotController_->setAxisNames(regression_->featureModel().getRawIds());
    updateFormulaText();
}

void ViewController::updateFormulaText()
{
    auto params = regression_->linearRegressionModel().getParams();
    auto featureNames = regression_->featureModel().getFeatureNames();
    auto response = regression_->featureModel().getResponseName();

    std::vector<std::string> summands;
    std::transform(params.begin(), params.end(),
                   featureNames.begin(),
                   std::back_inserter(summands),
                   [](double param, const std::string& name)
    {
        if (!name.empty())
            return boost::str(boost::format("%.2f*%s") % param % name);
        return boost::str(boost::format("%.2f") % param);
    });

    std::string expr = boost::algorithm::join(summands, " + ");

    lineEdit_->setText(to_qt(response + " = " + expr));
}

void ViewController::onDatasetUpdated()
{
    regression_->featureModel().update();
    regression_->linearRegressionModel().update();
}

