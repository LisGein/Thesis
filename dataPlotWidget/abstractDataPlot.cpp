#include "common/common.h"
#include "abstractDataPlot.h"

#include "../documentTree/featureModel.h"
#include "../documentTree/linearRegressionModel.h"

#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>


const int AbstractDataPlot::GRID_SIZE = 20;

using namespace DataPlot;
AbstractDataPlot::AbstractDataPlot(QWidget* parent)
	: QWidget(parent)
	, linearRegression_(nullptr)
	, axisXCombo_(std::make_unique<ComboBox>(QObject::tr("Axis x:")))
	, axisZCombo_(std::make_unique<ComboBox>(QObject::tr("Axis y:")))
{

	QWidget* axisWidget = new QWidget(this);
	QGridLayout* axisLay = new QGridLayout(axisWidget);



	axisLay->addWidget(axisXCombo_->label, 0, 0);
	axisLay->addWidget(axisXCombo_->box, 0, 1);

	axisLay->addWidget(axisZCombo_->label, 1, 0);
	axisLay->addWidget(axisZCombo_->box, 1, 1);

	axisWidget->setLayout(axisLay);


	QHBoxLayout* plotLay = new QHBoxLayout(this);
	plotLay->addWidget(axisWidget);
	setLayout(plotLay);

	connect(axisXCombo_->box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRegression()));
	connect(axisZCombo_->box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRegression()));

}

AbstractDataPlot::~AbstractDataPlot()
{

}

void AbstractDataPlot::setRegression(LinearRegressionModel* linearRegression)
{
	linearRegression_ = linearRegression;
	updateRegression();
}

void AbstractDataPlot::setAxisNames(const std::set<int> &axisIds)
{
	axisIds_ = axisIds;
	axisXCombo_->box->clear();
	axisZCombo_->box->clear();

	const FeatureModel& model = linearRegression_->getFeatureModel();
	for (const auto& feature : axisIds_)
	{
		QString name = to_qt(model.idTofeatureName(feature));
		axisXCombo_->box->addItem(name, QVariant(feature));
		axisZCombo_->box->addItem(name, QVariant(feature));
	}
	axisZCombo_->box->setCurrentIndex(1);
}

void AbstractDataPlot::updateRegression()
{
	if (linearRegression_ && axisXCombo_->box->currentIndex() != -1)
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
