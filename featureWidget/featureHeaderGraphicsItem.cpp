#include "common/common.h"
#include "featureHeaderGraphicsItem.h"
#include "documentTree/featureModel.h"

#include <QPainter>


FeatureHeaderGraphicsItem::FeatureHeaderGraphicsItem(const FeatureModel& featureModel, const QRect& rect, int id, QGraphicsItem *parent)
	: QGraphicsItem (parent)
	, featureModel_(featureModel)
	, rect_(rect)
	, id_(id)
{

}

FeatureHeaderGraphicsItem::~FeatureHeaderGraphicsItem()
{

}

QRectF FeatureHeaderGraphicsItem::boundingRect() const
{
	return rect_;
}

void FeatureHeaderGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* parent)
{
	Q_UNUSED(parent)
	QRectF rect = boundingRect();

	auto br = QBrush(QColor(123, 104, 238));
	painter->fillRect(rect, br);

	auto newpen = QPen(painter->pen());
	newpen.setColor(Qt::black);
	newpen.setWidthF(1.0);
	painter->setPen(newpen);

	painter->drawText(rect, to_qt(featureModel_.getRawFeatures()[id_]));
}
