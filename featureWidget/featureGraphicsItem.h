#pragma once
#include <QGraphicsItem>

class FeatureModel;

class FeatureGraphicsItem: public QGraphicsItem
{

public:
	FeatureGraphicsItem(const FeatureModel& featureModel,
							  const QRect& rect,
							  const QPair<int, int>& parents,
							  QGraphicsItem *parent = 0);

	virtual ~FeatureGraphicsItem();

	virtual QRectF boundingRect() const;

	void setChecked(bool is);
	bool isChecked() const;

	bool isDisabled() const;
	void setDisabled();

	const QPair<int, int> parents() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget* parent = 0);


private:
	QPair<int, int> parents_;
	bool isChecked_;
	QRect	rect_;
	bool isDisabled_;

	const FeatureModel& featureModel_;
};
