#pragma once
#include <QGraphicsScene>


class FeatureGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:

	FeatureGraphicsScene(QObject *parent = 0);
	virtual ~FeatureGraphicsScene();

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
