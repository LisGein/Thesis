#include "featureGraphicsScene.h"
#include "featureGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>


FeatureGraphicsScene::FeatureGraphicsScene(QObject* parent)
	: QGraphicsScene(parent)
	, sizeButton_(50)
{
	setBackgroundBrush(QBrush(Qt::lightGray));
}

FeatureGraphicsScene::~FeatureGraphicsScene()
{

}

void FeatureGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QList<QGraphicsItem *> sceneItems = items(event->scenePos());
	for (auto &it: sceneItems)
	{
		FeatureGraphicsItem *item = static_cast<FeatureGraphicsItem *>(it);
		if (item && item->isCheckable())
		{
			if (item->isCheck())
			{
				emit deletedFromFormula(item->parents());
				item->setCheck(false);
			}
			else
			{
				emit addedToFormula(item->parents());
				item->setCheck(true);
			}
			update();
		}
	}
}

void FeatureGraphicsScene::createNameItem(int x, int y, const QString& data)
{
	FeatureGraphicsItem *item = new FeatureGraphicsItem(QRect(x, y, sizeButton_ - 1, sizeButton_ - 1));
	item->setData(data);
	buttons_.push_back(item);
	addItem(item);

}

void FeatureGraphicsScene::updateTable(const QStringList& params)
{
	for (auto &it: buttons_)
		delete it;
	buttons_.clear();

	QRect rect(sizeButton_, sizeButton_, sizeButton_ - 1, sizeButton_ - 1);
	{
		int pos = sizeButton_;
		for(auto &it_x: params)
		{
			createNameItem(pos, 0, it_x);
			createNameItem(0, pos, it_x);
			pos += sizeButton_;
		}

	}


	int max = 1;
	int iter = 0;
	for(auto &it_x: params)
	{
		for(auto &it_y: params)
		{
			FeatureGraphicsItem *item = new FeatureGraphicsItem(rect, QPair<QString, QString>(it_x, it_y));
			if (iter >= max)
				item->setDisable();
			buttons_.push_back(item);
			addItem(item);
			rect.moveTo(rect.x() + sizeButton_, rect.y());
			++iter;
		}
		iter = 0;
		++max;
		rect.moveTo(sizeButton_, rect.y() + sizeButton_);
	}
}
