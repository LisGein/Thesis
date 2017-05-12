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
class DataPlotWidget;
class Data3DPlotWidget;

class ViewController : public QWidget
{
	Q_OBJECT

public:
	ViewController(QWidget* parent = 0);
	virtual ~ViewController();

	void setRegression(Regression* regression);
	bool containRegression() const;
	void update();


private slots:
	void deleteFromFormula(QPair<int, int> feature);
	void addToFormula(QPair<int, int> feature);


private:
	QGraphicsView* gview_;
	QLineEdit* lineEdit_;
	DataPlotWidget* dataPlotWidget_;
	Data3DPlotWidget* data3DPlotWidget_;
	Regression* regression_;


	void updateRegression();
	void updateFormulaText();
	void onDatasetUpdated();
};
