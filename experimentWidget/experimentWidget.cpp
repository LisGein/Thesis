#include "common/common.h"
#include "experimentWidget.h"

#include "featureFilterModel.h"

#include "documentTree/dataset.h"
#include "documentTree/experiment.h"


#include <QSignalBlocker>
#include "ui_experimentWidget.h"


ExperimentWidget::ExperimentWidget(QWidget* parent)
	: QWidget(parent)
	, ui_(std::make_unique<Ui::ExperimentWidget>())
	, experiment_(nullptr)
{
	ui_->setupUi(this);

	QObject::connect(ui_->response, SIGNAL(currentTextChanged(const QString &)),this, SLOT(updateResponse(const QString &)));
}

ExperimentWidget::~ExperimentWidget()
{

}

void ExperimentWidget::setExperiment(Experiment* experiment)
{
	experiment_ = experiment;
	if(experiment_)
	{
		featureFilterModel_ = std::make_unique<FeatureFilterModel>(*experiment);
		ui_->featureFilter->setModel(featureFilterModel_.get());
		QStringList features = to_qt(experiment_->getDataset().getNames());
		{
			const QSignalBlocker blocker(ui_->response);
			ui_->response->clear();
			ui_->response->addItems(features);

			int resp = experiment_->getResponseColumn();
			ui_->response->setCurrentIndex(resp);
		}
		ui_->analisys->setFeatures(features, experiment_->getDataset());
	}
}

void ExperimentWidget::updateResponse(const QString &)
{
	auto response = ui_->response->currentIndex();
	featureFilterModel_->setResponse(response);
}


