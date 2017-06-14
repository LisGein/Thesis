#include "common/common.h"
#include "chartSettings.h"
#include "documentTree/featureModel.h"
#include "ui_chartSettings.h"

#include <cassert>

#include <QSignalBlocker>
#include <QComboBox>


ChartSettings::ChartSettings(QWidget *parent)
	: QWidget(parent)
	, ui(std::make_unique<Ui::ChartSettings>())
	, model_(nullptr)
	, isHideY_(false)
{
	ui->setupUi(this);
	QObject::connect(ui->boxX, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOtherAxisNames()));
	QObject::connect(ui->boxY, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOtherAxisNames()));
	QObject::connect(ui->otherAxis, SIGNAL(currentIndexChanged(int)), this, SLOT(updateRange()));
}

ChartSettings::~ChartSettings()
{

}

void ChartSettings::hideYBox()
{
	isHideY_ = true;
	ui->labelY->hide();
	ui->boxY->hide();
}

void ChartSettings::clearAxis()
{
	const QSignalBlocker blockerX(ui->boxX);
	const QSignalBlocker blockerY(ui->boxY);
	const QSignalBlocker blockerOtherAxis(ui->otherAxis);
	ui->boxX->clear();
	ui->boxY->clear();
	ui->otherAxis->clear();
	ui->doubleSpinBox->setValue(0.0);
	ui->doubleSpinBox->setRange(-1.0, 1.0);

}

void ChartSettings::setAxisNames(const std::map<int, std::pair<double, double> > &axisIds)
{
	clearAxis();
	axisIds_ = axisIds;

	updateAxis();
}

void ChartSettings::setModel(const FeatureModel *model)
{
	model_ = model;
	axisIds_.clear();
	updateAxis();
}

void ChartSettings::updateAxis()
{
	if (model_ == nullptr)
		return;

	int i = 0;
	const QSignalBlocker blockerX(ui->boxX);
	const QSignalBlocker blockerY(ui->boxY);
	const QSignalBlocker blockerOtherAxis(ui->otherAxis);

	for (const auto& feature : axisIds_)
	{
		QString name = to_qt(model_->idTofeatureName(feature.first));
		ui->boxX->addItem(name, QVariant(feature.first));
		ui->boxY->addItem(name, QVariant(feature.first));
		if (i > 1)
		{
			ui->otherAxis->addItem(name, QVariant(feature.first));
			if (i == 2)
				updateRange();
		}
		++i;
	}
	if(!isHideY_)
		ui->boxY->setCurrentIndex(1);

}

void ChartSettings::updateRange()
{
	if (model_ == nullptr)
		return;

	int id = ui->otherAxis->currentData().toInt();
	auto iter = axisIds_.find(id);

	assert(iter != axisIds_.end());
	if (iter != axisIds_.end())
	{
		double min = iter->second.first;
		double max = iter->second.second;

		if (0.0 < min && 0.0 < max)
			min = 0.0;
		else if (0.0 > min && 0.0 > max)
			max = 0.0;


		ui->doubleSpinBox->setRange(min, max);
		ui->doubleSpinBox->setValue(0.0);
	}
}

void ChartSettings::updateOtherAxisNames()
{
	int idX = ui->boxX->currentData().toInt();
	int idY = ui->boxY->currentData().toInt();
	int idOther = ui->otherAxis->currentData().toInt();

	int index = 0;
	{
		const QSignalBlocker blockerOtherAxis(ui->otherAxis);
		ui->otherAxis->clear();

		ui->doubleSpinBox->setRange(-1.0, 1.0);
		ui->doubleSpinBox->setValue(0.0);

		int i =0;
		for (const auto& feature : axisIds_)
		{
			if (feature.first != idX && (feature.first != idY || isHideY_))
			{
				QString name = to_qt(model_->idTofeatureName(feature.first));
				ui->otherAxis->addItem(name, QVariant(feature.first));
				if (idOther == feature.first)
					index = i;
				++i;
			}
		}
	}

	ui->otherAxis->setCurrentIndex(index);
}
