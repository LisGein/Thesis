#pragma once
#include <QGraphicsScene>


class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:

	GraphicsScene(QObject *parent = 0);
	virtual ~GraphicsScene();

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
	void updateTable(const QStringList& params);

signals:
	void deletedFromFormula(QPair<QString, QString> parents);
	void addedToFormula(QPair<QString, QString> parents);

private:
	QVector< QGraphicsItem* > buttons_;
	int sizeButton_;

	void createNameItem(int x, int y, const QString& data);

};
