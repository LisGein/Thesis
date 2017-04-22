#include "common/common.h"
#include "mainWidget.h"
#include "datasetWidget/datasetView.h"
#include "datasetWidget/datasetModel.h"
#include "document.h"
#include "dataset.h"
#include "featureModel.h"
#include "featureWidget/featureGraphicsScene.h"
#include "regression.h"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QLineEdit>

#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>



MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, formulaEdit_(new QLineEdit(this))
	, datasetView_(new DatasetView(this))
	, document_(std::make_unique<Document>())
{
	setLayout(new QVBoxLayout(this));

	layout()->addWidget(datasetView_);

	QGraphicsView *gview = new QGraphicsView(this);
	gview->setScene(document_->getFeaturesModel().getScene());
	layout()->addWidget(gview);

	datasetView_->setModel(&document_->getDatasetModel());

	layout()->addWidget(formulaEdit_);


	QObject::connect(datasetView_, SIGNAL(insertedTable(QString)), this, SLOT(onInsertTable(QString)));
	QObject::connect(document_->getFeaturesModel().getScene(), SIGNAL(deletedFromFormula(QPair<int, int>)), this, SLOT(deleteFromFormula(QPair<int, int>)));
	QObject::connect(document_->getFeaturesModel().getScene(), SIGNAL(addedToFormula(QPair<int, int>)), this, SLOT(addToFormula(QPair<int, int>)));
}

MainWidget::~MainWidget()
{

}

void MainWidget::onInsertTable(QString str)
{
	document_->getDatasetModel().beginReset();
	document_->getDataset().loadFromTsv(str.toStdString());
	document_->getDatasetModel().endReset();

	onDatasetUpdated();
}

void MainWidget::deleteFromFormula(QPair<int, int> feature)
{
	document_->getFeaturesModel().removeFeature(from_qt(feature));
	updateRegression();
}

void MainWidget::addToFormula(QPair<int, int> feature)
{
	document_->getFeaturesModel().addFeature(from_qt(feature));
	updateRegression();
}

void MainWidget::updateRegression()
{
	document_->getLinearRegressionModel().update();
	updateFormulaText();
}

void MainWidget::updateFormulaText()
{
	auto params = document_->getLinearRegressionModel().getParams();
	auto featureNames = document_->getFeaturesModel().getFeatureNames();
	auto response = document_->getFeaturesModel().getResponseName();

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

	formulaEdit_->setText(to_qt(response + " = " + expr));
}

void MainWidget::onDatasetUpdated()
{
	document_->getFeaturesModel().update();
	document_->getLinearRegressionModel().update();
}

