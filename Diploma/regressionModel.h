#pragma once
#include <QGraphicsScene>


class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:

	GraphicsScene(QObject *parent = 0);
	virtual ~GraphicsScene();

public slots:
	void updateTable(const QStringList& params);

private:
	QVector< QGraphicsItem* > buttons_;
	int sizeButton_;

	void createNameItem(int x, int y, const QString& data);

};
