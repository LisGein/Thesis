#pragma once
#include <QWidget>

class GraphicsScene;

class MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = 0);
	virtual ~MainWidget();

private slots:
	void convertInsertedTable(QStringList list);

private:
	GraphicsScene* scene_;
};
