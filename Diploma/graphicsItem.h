#pragma once
#include <QGraphicsItem>

class GraphicsItem: public QGraphicsItem
{

public:
	GraphicsItem(const QRect& rect,
					 const QPair<QString, QString>& parents = QPair<QString, QString>(),
					 QGraphicsItem *parent = 0);
	virtual ~GraphicsItem();

	virtual QRectF boundingRect() const;
	void setRect();

	void setCheck(bool is);
	bool isCheck() const;

	void setData(const QString& data);
	void setDisable();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
	QPair<QString, QString> parents_;
	bool isCheck_;
	QString data_;
	QRect	rect_;
	bool disable_;

};
