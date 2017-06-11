#include "common/common.h"
#include "datasetWidget.h"
#include "ui_datasetWidget.h"

#include "datasetModel.h"

#include <QMenuBar>
#include <QBoxLayout>
#include <QFileDialog>
#include <QTextEdit>
#include <cstring>

extern "C"{
#include "xlsxio_read.h"
}

DatasetWidget::DatasetWidget()
	: ui_(std::make_unique<Ui::DatasetWidget>())
{
	ui_->setupUi(this);

	QObject::connect(ui_->excelButton, SIGNAL(released()), this, SLOT(loadFromXls()));
	QObject::connect(ui_->tsvButton, SIGNAL(released()), this, SLOT(loadFromTsv()));

	QString str = QObject::tr("Description of the document") + "\n";
	ui_->descriptionDocument->setText(str);
}

DatasetWidget::~DatasetWidget()
{

}

void DatasetWidget::openRegression(boost::property_tree::ptree &inventoryTree)
{
	ui_->descriptionDocument->clear();
	std::string text = inventoryTree.get<std::string>("description", "");
	ui_->descriptionDocument->setText(QString::fromStdString(text));
}

void DatasetWidget::saveRegression(boost::property_tree::ptree &inventoryTree)
{
	inventoryTree.put("description", ui_->descriptionDocument->toPlainText().toStdString());
}

void DatasetWidget::setDatasetModel(DatasetModel* model)
{
	ui_->datasetView->setModel(model);

	QObject::connect(ui_->datasetView, SIGNAL(insertedTable(std::string)), this, SIGNAL(updatedTable(std::string)));
}

void DatasetWidget::loadFromXls()
{
	QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"),"", "Files (*.xls *.xlsx)");

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
	QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"),"","Files (*.tsv)");
	std::ifstream file(fileName.toStdString());
	if ( file )
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		emit updatedTable(buffer.str());
		file.close();
	}
}
