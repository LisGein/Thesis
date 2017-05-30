#include "common/common.h"
#include "datasetWidget.h"
#include "ui_datasetWidget.h"

#include "datasetModel.h"

#include <QMenuBar>
#include <QBoxLayout>
#include <QFileDialog>

#include <cstring>

extern "C"{
#include "xlsxio_read.h"
}

DatasetWidget::DatasetWidget()
	: menu_(new QMenuBar(this))
	, datasetView_(new DatasetView(this))
	, featureParams_(new QTableView(this))
{
	setLayout(new QVBoxLayout());

	QAction *excel = new QAction(QObject::tr("Load from Excel"), menu_);
	menu_->addAction(excel);
	QObject::connect(excel, SIGNAL(triggered(bool)), this, SLOT(loadFromXls()));

	QAction *tsv = new QAction(QObject::tr("Load from Tsv"), menu_);
	menu_->addAction(tsv);
	QObject::connect(tsv, SIGNAL(triggered(bool)), this, SLOT(loadFromTsv()));

	layout()->addWidget(menu_);
	layout()->addWidget(datasetView_);
	layout()->addWidget(featureParams_);
}

DatasetWidget::~DatasetWidget()
{

}

void DatasetWidget::setDatasetModel(DatasetModel* model)
{
	datasetView_->setModel(model);

	QObject::connect(datasetView_, SIGNAL(insertedTable(std::string)), this, SIGNAL(updatedTable(std::string)));
}

void DatasetWidget::loadFromXls()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.xls *.xlsx)"));

	xlsxioreader xlsxioread;
	if ((xlsxioread = xlsxioread_open(fileName.toStdString().c_str())) == NULL)
		return;

	std::string tableData;
	xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioread, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
	bool endOfFile = false;
	while (xlsxioread_sheet_next_row(sheet) && !endOfFile)
	{
		char* value;
		while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL && !endOfFile)
		{
			endOfFile = (strcmp(value, "") == 0);
			if (endOfFile)
				break;

			tableData.append(value);
			tableData.append("\t");
			free(value);
		}
		auto it = tableData.end() -1;
		if (it != tableData.end())
			tableData.erase(it);

		tableData.append("\n");
	}
	xlsxioread_sheet_close(sheet);
	xlsxioread_close(xlsxioread);
	emit updatedTable(tableData);
}

void DatasetWidget::loadFromTsv()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.tsv)"));
	std::ifstream file(fileName.toStdString());
	if ( file )
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		emit updatedTable(buffer.str());
		file.close();
	}
}
