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

    QObject::connect(ui_->response, SIGNAL(currentTextChanged(const QString &)),this, SLOT(updateResponse(const QString &)));
    QObject::connect(ui_->feature, SIGNAL(featuresChanged()), this, SLOT(updateFiltredDataset()));
}

ExperimentWidget::~ExperimentWidget()
{

}

void ExperimentWidget::setExperiment(Experiment* experiment)
{
    experiment_ = experiment;
    if(experiment_)
    {
        int resp = experiment_->getResponseColumn();
        std::vector<int> feature = experiment_->filtredDataset().features();


        ui_->response->clear();

        QStringList features = to_qt(experiment_->getDataset().getNames());

        ui_->response->addItems(features);

        int originalResponse = experiment_->filtredDataset().originColumns(0);
        ui_->response->setCurrentIndex(originalResponse);

        ui_->feature->setFeatures(features, experiment_->getEnabledFeatures());
        ui_->analisys->setFeatures(features, experiment_->getDataset());

        ui_->response->setCurrentIndex(resp);
        ui_->feature->updateFeature(feature);
        ui_->feature->update();
    }
}

void ExperimentWidget::updateFiltredDataset()
{
    const auto& checkedFeatures = ui_->feature->checkedFeatures();

    if(experiment_)
    {
        experiment_->setEnabledFeatures(checkedFeatures);
        experiment_->setResponse(ui_->response->currentIndex());
    }
}

void ExperimentWidget::updateResponse(const QString &name)
{
    ui_->feature->disableFeature(name);
    updateFiltredDataset();
}


