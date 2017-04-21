#include "graphicsItem.h"
#include <QPainter>
#include <QDebug>


GraphicsItem::GraphicsItem(const QRect& rect, const QPair<QString, QString>& parents, QGraphicsItem* parent)
	: QGraphicsItem (parent)
	, parents_(parents)
	, isCheck_(false)
	, rect_(rect)
	, disable_(false)
	, isHeader_(parents.first.isEmpty() && parents.first.isEmpty())
{
}

GraphicsItem::~GraphicsItem()
{

}

void GraphicsItem::setCheck(bool is)
{
	isCheck_ = is;
}
QRectF GraphicsItem::boundingRect() const
{
	return rect_;
}

bool GraphicsItem::isCheck() const
{
	return isCheck_;
}

bool GraphicsItem::isCheckable() const
{
	return (parents_.first.size());
}

void GraphicsItem::setData(const QString& data)
{
	data_ = data;
}

void GraphicsItem::setDisable()
{
	disable_ = true;
}

const QPair<QString, QString> GraphicsItem::parents() const
{
	return parents_;
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect = boundingRect();

	if (disable_)
		painter->fillRect(rect, Qt::Dense5Pattern);
	else
	{
		QBrush br = isHeader_? QColor(123, 104, 238) : (isCheck_ ? Qt::darkGray : Qt::gray);
		painter->fillRect(rect, br);

		QPen newpen(painter->pen());
		newpen.setColor(Qt::black);
		newpen.setWidthF(1.0);
		painter->setPen(newpen);

		if (data_.size())
			painter->drawText(rect, data_);
		else
			painter->drawText(rect, parents_.first + " - " + parents_.second);

	}
}
