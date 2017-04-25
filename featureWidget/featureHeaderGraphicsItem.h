#pragma once
#include <QGraphicsItem>

class FeatureModel;

class FeatureHeaderGraphicsItem: public QGraphicsItem
{
public:
	FeatureHeaderGraphicsItem(const FeatureModel& featureModel, const QRect& rect, int id, QGraphicsItem *parent = 0);
	~FeatureHeaderGraphicsItem();

	QRectF boundingRect() const override;
	void setRect();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget* parent = 0) override;

private:
	const FeatureModel& featureModel_;
	QRect	rect_;
	int id_;
};
