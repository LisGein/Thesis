#include "common/common.h"
#include "abstractDataPlot.h"
#include "chartSettings.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QComboBox>
#include <QLabel>
#include <QSplitter>
#include <QHBoxLayout>

#include "ui_chartSettings.h"


const int AbstractDataPlot::GRID_SIZE = 20;

using namespace DataPlot;
AbstractDataPlot::AbstractDataPlot(QWidget* parent)
	: QWidget(parent)
	, linearRegression_(nullptr)
	, chartSettings_(std::make_unique<ChartSettings>())
	, axisXCombo_(std::make_unique<ComboBox>(QObject::tr("Axis x:")))
	, axisZCombo_(std::make_unique<ComboBox>(QObject::tr("Axis y:")))
	, splitter_(new QSplitter(this))
{
	chartSettings_->adjustSize();
	splitter_->addWidget(chartSettings_.get());
	QHBoxLayout* plotLay = new QHBoxLayout(this);
	layout()->addWidget(splitter_);
	setLayout(plotLay);

	connect(chartSettings_->ui->boxX, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRegression()));
	connect(chartSettings_->ui->boxY, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRegression()));
	connect(chartSettings_->ui->otherAxis, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRegression()));
	connect(chartSettings_->ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateRegression()));

}

AbstractDataPlot::~AbstractDataPlot()
{

}

void AbstractDataPlot::setRegression(LinearRegressionModel* linearRegression)
{
	linearRegression_ = linearRegression;
	chartSettings_->setModel(&(linearRegression_->getFeatureModel()));
	updateRegression();
}

void AbstractDataPlot::setAxisNames(const std::map<int, std::pair<double, double>> &axisIds)
{
	axisIds_ = axisIds;
	chartSettings_->clearAxis();
	chartSettings_->setAxisNames(axisIds);
}

void AbstractDataPlot::updateRegression()
{
	if (linearRegression_ && chartSettings_->ui->boxX->currentIndex() != -1)
	{
		auto data = linearRegression_->getFeatureModel().data();
		auto resp = linearRegression_->getFeatureModel().responses();

		if (data.n_cols == 0 || data.n_rows == 0)
			clear();
		else
			updateChart(data, resp);
	}
}

std::pair<double, double> AbstractDataPlot::bounds(const arma::mat &data, const arma::vec &column) const
{
	std::pair<double, double> xBounds;

	for (size_t i = 0; i < data.n_rows; ++i)
	{
		auto x = column.at(i);

		if (i == 0)
			xBounds = std::make_pair(x, x);
		else
		{
			if (x < xBounds.first)
				xBounds.first = x;
			if (x > xBounds.second)
				xBounds.second = x;
		}
	}

	return xBounds;
}

ComboBox::ComboBox(const QString &text)
	: box(new QComboBox())
	, label(new QLabel(text))
{}

ComboBox::~ComboBox()
{
	delete box;
	delete label;
}

void ComboBox::hide()
{
	label->hide();
	box->hide();
}
