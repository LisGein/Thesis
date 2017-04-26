#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "treeWidget/treeModel.h"
#include "treeWidget/treeView.h"

#include "documentTree/iNode.h"
#include "documentTree/document.h"
#include "documentTree/regression.h"
#include "documentTree/experiment.h"
#include "documentTree/featureModel.h"

#include "datasetWidget/datasetWidget.h"
#include "featureWidget/featureGraphicsScene.h"
#include "experimentWidget/experimentWidget.h"
#include "dataPlotWidget/dataPlotWidget.h"

#include <QSettings>
#include <QLineEdit>
#include <QGraphicsView>

#include "viewController.h"



MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, appSettings_(std::make_unique<QSettings>())
	, document_(std::make_unique<Document>())
	, fakeRoot_(std::make_unique<Root>(*document_))
	, documentModel_(std::make_unique<TreeModel>(fakeRoot_.get()))
	, datasetWidget_(std::make_unique<DatasetWidget>())
	, experimentWidget_(std::make_unique<ExperimentWidget>(this))
	, graphicsController_(std::make_unique<ViewController>(this))
	, ui_(std::make_unique<Ui::MainWindow>())
{
	ui_->setupUi(this);

	auto size = appSettings_->value("WindowSize", QSize(640, 480));
	resize(size.value<QSize>());


	ui_->documentView->setModel(documentModel_.get());
	connect(ui_->documentView, SIGNAL(childAdded(const QModelIndex&)), documentModel_.get(), SLOT(onAddNewChild(const QModelIndex&)));

	ui_->stackedWidget->addWidget(datasetWidget_.get());
	datasetWidget_->setDatasetModel(&document_->getDatasetModel());

	QObject::connect(ui_->documentView, SIGNAL(changedCurrentWidget(INode*)), this, SLOT(changeCurrentWidget(INode*)));
	QObject::connect(datasetWidget_.get(), SIGNAL(insertedTable(QString)), this, SLOT(insertTable(QString)));

	ui_->stackedWidget->addWidget(experimentWidget_.get());
	ui_->stackedWidget->addWidget(graphicsController_.get());
}

MainWindow::~MainWindow()
{
	appSettings_->setValue("WindowSize", size());
}

void MainWindow::changeCurrentWidget(INode* node)
{
	INode::TypeObject typeObj(static_cast<INode::TypeObject>(node->type()));

	if (ui_->stackedWidget->currentIndex() == 1)
		experimentWidget_->updateFiltredDataset();

	switch (typeObj)
	{
	case INode::TypeObject::Document:
		ui_->stackedWidget->setCurrentIndex(0);
		break;
	case INode::TypeObject::Experiment:
		ui_->stackedWidget->setCurrentIndex(1);
		if (auto* experiment = dynamic_cast<Experiment *>(node))
			experimentWidget_->setExperiment(experiment);
		break;
	case INode::TypeObject::Regression:
		ui_->stackedWidget->setCurrentIndex(2);
		if(!graphicsController_->containRegression())
		{
			if (auto* regression = dynamic_cast<Regression *>(node))
				graphicsController_->setRegression(regression);

		}
		graphicsController_->update();
		break;
	default:
		break;
	}

}

void MainWindow::insertTable(QString str)
{
	document_->loadFromTsv(str.toStdString());
}


