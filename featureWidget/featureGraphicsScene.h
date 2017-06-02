#pragma once
#include <QGraphicsScene>


class FeatureModel;

class FeatureGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:

	FeatureGraphicsScene(FeatureModel& featureModel, QObject *parent = 0);
	virtual ~FeatureGraphicsScene();

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
	void updateTable(int nRawFeatures);

signals:
	void deletedFromFormula(QPair<int, int> parents);
	void addedToFormula(QPair<int, int> parents);
	void updatedRegression();

private:
	void createHeadertem(int x, int y, int id);

private:
	QVector< QGraphicsItem* > buttons_;
	int buttonSize_;
	FeatureModel& featureModel_;
};
