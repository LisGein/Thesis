#include "common/common.h"
#include "experimentWidget.h"
#include "ui_experimentWidget.h"
#include "documentTree/dataset.h"
#include "documentTree/experiment.h"



ExperimentWidget::ExperimentWidget(QWidget* parent)
	: QWidget(parent)
	, ui_(std::make_unique<Ui::ExperimentWidget>())
	, experiment_(nullptr)
{
	ui_->setupUi(this);

	QObject::connect(ui_->response, SIGNAL(currentTextChanged(const QString &)), ui_->feature, SLOT(disableFeature(const QString &)));
	QObject::connect(ui_->feature, SIGNAL(featuresChanged()), this, SLOT(updateFiltredDataset()));
}

ExperimentWidget::~ExperimentWidget()
{

}

void ExperimentWidget::setExperiment(Experiment* experiment)
{
	experiment_ = experiment;

	ui_->response->clear();

	QStringList features = to_qt(experiment_->getDataset().getNames());

	ui_->response->addItems(features);

	auto filteredResponse = experiment_->getResponseColumn();
	auto originalResponse = *std::next(experiment_->getEnabledFeatures().begin(), filteredResponse);
	ui_->response->setCurrentIndex(originalResponse);

	ui_->feature->setFeatures(features, experiment_->getEnabledFeatures());
	ui_->feature->disableFeature(ui_->response->currentText());
}

void ExperimentWidget::updateFiltredDataset()
{
	const auto& checkedFeatures = ui_->feature->checkedFeatures();

	if(experiment_)
	{
		experiment_->setEnabledFeatures(checkedFeatures);
		auto originalResponse = ui_->response->currentIndex();
		experiment_->setResponse(std::distance(experiment_->getEnabledFeatures().begin(), experiment_->getEnabledFeatures().find(originalResponse)));
	}
}


