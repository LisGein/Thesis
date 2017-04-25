#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "treeWidget/treeModel.h"
#include "treeWidget/treeView.h"

#include "documentTree/document.h"
#include "documentTree/iNode.h"

#include "datasetWidget/datasetWidget.h"
#include "featureWidget/featureGraphicsScene.h"
#include "documentTree/featureModel.h"

#include <QSettings>
#include <QLineEdit>
#include <QGraphicsView>

#include "dataPlotWidget/dataPlotWidget.h"
#include "viewController.h"
#include "experimentWidget/experimentWidget.h"


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

	QObject::connect(ui_->documentView, SIGNAL(changedCurrentWidget(int)), this, SLOT(changeCurrentWidget(int)));
	QObject::connect(datasetWidget_.get(), SIGNAL(insertedTable(QString)), this, SLOT(insertTable(QString)));

	ui_->stackedWidget->addWidget(experimentWidget_.get());
	ui_->stackedWidget->addWidget(graphicsController_.get());
}

MainWindow::~MainWindow()
{
	appSettings_->setValue("WindowSize", size());
}

void MainWindow::changeCurrentWidget(int type)
{
	INode::TypeObject typeObj(static_cast<INode::TypeObject>(type));
	if (typeObj != INode::TypeObject::Invalid)
	{
		ui_->stackedWidget->setCurrentIndex(int(typeObj) - 1);
		experimentWidget_->setDataset(&(document_->getDataset()));
		//graphicsController_->setScene(FeatureGraphicsScene());
	}
}

void MainWindow::insertTable(QString str)
{
	document_->loadFromTsv(str.toStdString());
}


