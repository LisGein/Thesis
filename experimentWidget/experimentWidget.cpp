#include "common/qt.h"
#include "experimentWidget.h"
#include "ui_experimentWidget.h"
#include "documentTree/dataset.h"


ExperimentWidget::ExperimentWidget(QWidget* parent)
	: QWidget(parent)
	, ui_(std::make_unique<Ui::ExperimentWidget>())
	, dataset_(nullptr)
{
	ui_->setupUi(this);
}

ExperimentWidget::~ExperimentWidget()
{

}

void ExperimentWidget::setDataset(Dataset* dataset)
{
	ui_->response->clear();
	dataset_ = dataset;

	QStringList feature = to_qt(dataset_->getFeatures());
	ui_->feature->setFeatures(feature);

	ui_->response->addItems(feature);

	QObject::connect(ui_->response, SIGNAL(currentTextChanged(const QString &)), ui_->feature, SLOT(removeItem(const QString &)));

}
