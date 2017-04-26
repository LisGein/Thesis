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

	QObject::connect(ui_->response, SIGNAL(currentTextChanged(const QString &)), ui_->feature, SLOT(removeItem(const QString &)));
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

	ui_->feature->setFeatures(features, experiment_->getEnabledFeatures());
	ui_->response->addItems(features);
}

void ExperimentWidget::updateFiltredDataset()
{
	const auto& checkedFeatures = ui_->feature->checkedFeatures();

	if(experiment_)
	{
		experiment_->setEnabledFeatures(checkedFeatures.second);
		experiment_->setResponse(checkedFeatures.first);
	}
}


