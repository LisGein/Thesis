#include "common/common.h"
#include "featureGraphicsItem.h"
#include "featureModel.h"

#include <QPainter>
#include <QDebug>


FeatureGraphicsItem::FeatureGraphicsItem(const FeatureModel& featureModel, const QRect& rect, const QPair<int, int>& parents, QGraphicsItem* parent)
	: QGraphicsItem (parent)
	, parents_(parents)
	, isChecked_(false)
	, rect_(rect)
	, isDisabled_(false)
	, featureModel_(featureModel)
{
}

FeatureGraphicsItem::~FeatureGraphicsItem()
{

}

void FeatureGraphicsItem::setChecked(bool is)
{
	isChecked_ = is;
}
QRectF FeatureGraphicsItem::boundingRect() const
{
	return rect_;
}

bool FeatureGraphicsItem::isChecked() const
{
	return isChecked_;
}

bool FeatureGraphicsItem::isDisabled() const
{
	return isDisabled_;
}

void FeatureGraphicsItem::setDisabled()
{
	isDisabled_ = true;
}

const QPair<int, int> FeatureGraphicsItem::parents() const
{
	return parents_;
}

void FeatureGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect = boundingRect();

	if (isDisabled_)
		painter->fillRect(rect, Qt::Dense5Pattern);
	else
	{
		QBrush br = isChecked_ ? Qt::green : Qt::gray;
		painter->fillRect(rect, br);

		QPen newPen(painter->pen());
		newPen.setColor(Qt::black);
		newPen.setWidthF(1.0);
		painter->setPen(newPen);

		painter->drawText(rect, to_qt(featureModel_.getFeatureName(from_qt(parents_), true)));

	}
}
