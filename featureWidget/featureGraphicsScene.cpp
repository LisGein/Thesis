#include "common/common.h"
#include "featureGraphicsScene.h"
#include "featureGraphicsItem.h"
#include "featureHeaderGraphicsItem.h"

#include "documentTree/featureModel.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>


FeatureGraphicsScene::FeatureGraphicsScene(FeatureModel& featureModel, QObject* parent)
	: QGraphicsScene(parent)
	, buttonSize_(50)
	, featureModel_(featureModel)
{
	setBackgroundBrush(QBrush(Qt::lightGray));
}

FeatureGraphicsScene::~FeatureGraphicsScene()
{

}

void FeatureGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	update();
	QList<QGraphicsItem *> sceneItems = items(event->scenePos());
	for (auto &it: sceneItems)
	{
		FeatureGraphicsItem *item = dynamic_cast<FeatureGraphicsItem *>(it);
		if (item && !item->isDisabled())
		{
			if (item->isChecked())
			{
				emit deletedFromFormula(item->parentsFeature());
				item->setChecked(false);
			}
			else
			{
				emit addedToFormula(item->parentsFeature());
				item->setChecked(true);
			}
			update();
			emit updatedRegression();
		}
	}
}

void FeatureGraphicsScene::updateChecked()
{
	auto& featureSet = featureModel_.featureSet();

	for (auto &it: buttons_)
	{
		FeatureGraphicsItem* item = dynamic_cast<FeatureGraphicsItem*>(it);
		if (item)
		{
			FeatureModel::Feature feature = from_qt(item->parentsFeature());
			item->setChecked(featureSet.find(feature) != featureSet.end());
		}
	}

}

void FeatureGraphicsScene::createHeadertem(int x, int y, int id)
{
	FeatureHeaderGraphicsItem *item = new FeatureHeaderGraphicsItem(featureModel_, QRect(x, y, buttonSize_ - 1, buttonSize_ - 1), id);
	buttons_.push_back(item);
	addItem(item);

}

void FeatureGraphicsScene::updateTable(int nRawFeatures)
{
	for (auto &it: buttons_)
		delete it;
	buttons_.clear();

	QRect rect(buttonSize_, buttonSize_, buttonSize_ - 1, buttonSize_ - 1);
	{
		int pos = buttonSize_;

		for(int i = 0; i < nRawFeatures; ++i)
		{
			createHeadertem(pos, 0, i);
			createHeadertem(0, pos, i);
			pos += buttonSize_;
		}

	}


	const std::set<FeatureModel::Feature> &featureSet = featureModel_.featureSet();

	for(int row = 0; row < nRawFeatures; ++row)
	{
		for(int col = 0; col < nRawFeatures; ++col)
		{
			FeatureGraphicsItem *item = new FeatureGraphicsItem(featureModel_, rect, qMakePair(row, col));
			if (col > row)
				item->setDisabled();
			item->setChecked(featureSet.find(FeatureModel::Feature(row, col)) != featureSet.end());
			buttons_.push_back(item);
			addItem(item);
			rect.moveTo(rect.x() + buttonSize_, rect.y());
		}
		rect.moveTo(buttonSize_, rect.y() + buttonSize_);
	}
}
