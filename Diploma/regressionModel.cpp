#include "regressionModel.h"
#include "graphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>


GraphicsScene::GraphicsScene(QObject* parent)
	: QGraphicsScene(parent)
	, sizeButton_(50)
{
	setBackgroundBrush(QBrush(Qt::lightGray));
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QList<QGraphicsItem *> sceneItems = items(event->scenePos());
	for (auto &it: sceneItems)
	{
		GraphicsItem *item = static_cast<GraphicsItem *>(it);
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
		}
	}
}

void GraphicsScene::createNameItem(int x, int y, const QString& data)
{
	GraphicsItem *item = new GraphicsItem(QRect(x, y, sizeButton_ - 1, sizeButton_ - 1));
	item->setData(data);
	buttons_.push_back(item);
	addItem(item);

}

void GraphicsScene::updateTable(const QStringList& params)
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
			GraphicsItem *item = new GraphicsItem(rect, QPair<QString, QString>(it_x, it_y));
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
