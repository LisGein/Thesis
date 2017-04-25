#pragma once
#include <QWidget>
#include <QMap>
#include <memory>

class FeatureGraphicsScene;
class QLineEdit;
class DatasetView;
class Document;
class QGraphicsView;


class ViewController : public QWidget
{
	Q_OBJECT

public:
	ViewController(QWidget* parent = 0);
	virtual ~ViewController();

	void setScene(FeatureGraphicsScene *scene);


private:
	QGraphicsView* gview_;
	QLineEdit* lineEdit_;
};
