#include "mainWidget.h"
#include <QVBoxLayout>
#include "tableView.h"
#include "regressionModel.h"
#include <QGraphicsView>
#include <QLineEdit>
#include <cassert>
#include <mlpack/methods/linear_regression/linear_regression.hpp>


MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, scene_(new GraphicsScene(this))
	, line_(new QLineEdit(this))
	, table_(new TableView(this))
{
	setLayout(new QVBoxLayout(this));

	layout()->addWidget(table_);
	layout()->addWidget(line_);

	QGraphicsView *gview = new QGraphicsView(this);
	gview->setScene(scene_);
	layout()->addWidget(gview);

	QObject::connect(table_, SIGNAL(insertedTable(QStringList)), this, SLOT(convertInsertedTable(QStringList)));
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

	auto it = addedParents_.find(parents.first);
	while(it != addedParents_.end() && it.key() == parents.first)
	{
		if (it.value() == parents.second)
		{
			it = addedParents_.erase(it);
		}
		else
			++it;
	}

	updateRegression();
}

void MainWidget::addToFormula(QPair<QString, QString> parents)
{
	QString text = generateTextForFormula(parents);
	QString symb = generateSymbForFormula();


	line_->setText(line_->text() + symb + text);

	lineText_.insert(text, symb);
	addedParents_.insert(parents.first, parents.second);

	updateRegression();

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

void MainWidget::updateRegression()
{

	using namespace mlpack::regression;

	QVector<double> vec;

	int column = 0;
	int row = 0;

	for (auto it = addedParents_.begin(); it != addedParents_.end(); ++it)
	{
		vec += table_->dataFromLines(it.key(), it.value());
		++ column;
		if (row == 0)
			row = vec.size();
	}

	arma::mat regressors = vec.toStdVector();
	regressors.reshape(row, column);

	regressors.print("Input data:");

	//regressors = regressors.t();

	regressors.print("Input data:");


	arma::vec responses = table_->dataFromLines("y", "1").toStdVector();
	responses.print("Responses:");

	LinearRegression lr(regressors.t(), responses, 0, false);
	arma::vec parameters = lr.Parameters();
	parameters.print("Params:");


}

