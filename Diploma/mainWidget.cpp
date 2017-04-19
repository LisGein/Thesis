#include "mainWidget.h"
#include <QVBoxLayout>
#include "tableWidget.h"
#include "regressionModel.h"
#include <QGraphicsView>
#include <QLineEdit>
#include <cassert>


MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, scene_(new GraphicsScene(this))
	, line_(new QLineEdit(this))
{
	setLayout(new QVBoxLayout(this));

	TableWidget *table = new TableWidget(this);
	table->setColumnCount(10);
	table->setRowCount(10);
	layout()->addWidget(table);

	layout()->addWidget(line_);

	QGraphicsView *gview = new QGraphicsView(this);
	gview->setScene(scene_);
	layout()->addWidget(gview);

	QObject::connect(table, SIGNAL(insertedTable(QStringList)), this, SLOT(convertInsertedTable(QStringList)));
	QObject::connect(scene_, SIGNAL(deletedFromFormula(QPair<QString, QString>)), this, SLOT(deleteFromFormula(QPair<QString, QString>)));
	QObject::connect(scene_, SIGNAL(addedToFormula(QPair<QString, QString>)), this, SLOT(addToFormula(QPair<QString, QString>)));

}

MainWidget::~MainWidget()
{

}

void MainWidget::convertInsertedTable(QStringList list)
{
	lineText_.clear();
	line_->clear();

	list.push_front("1");
	startFormula_ = (*(list.end() - 1)) + " = ";
	line_->setText(startFormula_);
	list.pop_back();
	scene_->updateTable(list);
}

void MainWidget::deleteFromFormula(QPair<QString, QString> parents)
{
	QString text = generateTextForFormula(parents);
	auto iter = lineText_.find(text);

	assert(iter != lineText_.end());
	lineText_.erase(iter);

	text = startFormula_;
	for (auto it = lineText_.begin(); it != lineText_.end(); ++it)
	{
		if (text == startFormula_ && it.value() == " + ")
				text  += it.key();

		else
			text  += it.value() + it.key();
	}
	line_->setText(text);

}

void MainWidget::addToFormula(QPair<QString, QString> parents)
{
	QString text = generateTextForFormula(parents);
	QString symb = generateSymbForFormula();


	line_->setText(line_->text() + symb + text);

	lineText_.insert(text, symb);

}

QString MainWidget::generateTextForFormula(const QPair<QString, QString>& parents) const
{
	QString text = parents.first;
	if (parents.second == "1")
		return text;

	if (parents.first == parents.second)
		text += " ^ 2";
	else
		text += " * " + parents.second;

	return text;
}

QString MainWidget::generateSymbForFormula() const
{
	QString symb = " + ";
	if (line_->text() == startFormula_ )
		symb = " ";
	return symb;
}

