#pragma once
#include <QWidget>
#include <QMap>
#include <memory>

class FeatureGraphicsScene;
class QLineEdit;
class DatasetView;
class Document;
class QGraphicsView;
class Regression;

class ViewController : public QWidget
{
	Q_OBJECT

public:
	ViewController(QWidget* parent = 0);
	virtual ~ViewController();

	void setRegression(Regression* regression);
	bool containRegression() const;
	void update();


private:
	QGraphicsView* gview_;
	QLineEdit* lineEdit_;
	Regression* regression_;
};
