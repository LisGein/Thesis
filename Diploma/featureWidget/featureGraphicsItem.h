#pragma once
#include <QGraphicsItem>

class FeatureGraphicsItem: public QGraphicsItem
{

public:
	FeatureGraphicsItem(const QRect& rect,
					 const QPair<QString, QString>& parents = QPair<QString, QString>(),
					 QGraphicsItem *parent = 0);
	virtual ~FeatureGraphicsItem();

	virtual QRectF boundingRect() const;
	void setRect();

	void setCheck(bool is);
	bool isCheck() const;

	bool isCheckable() const;

	void setData(const QString& data);
	void setDisable();

	const QPair<QString, QString> parents() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);


private:
	QPair<QString, QString> parents_;
	bool isCheck_;
	QString data_;
	QRect	rect_;
	bool disable_;
	bool isHeader_;

};
