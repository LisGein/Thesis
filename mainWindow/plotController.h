#pragma once
#include <QWidget>

#include <set>


class DataPlotWidget;
class Data3DPlotWidget;
class Regression;

class PlotController: public QWidget
{
	Q_OBJECT
public:
	PlotController(QWidget* parent = nullptr);
	virtual ~PlotController();

	void setRegression(Regression* regression);
	void setAxisNames(const std::map<int, std::pair<double, double> > &names);

private slots:
	void changeGraphic();

private:
	DataPlotWidget* dataPlotWidget_;
	Data3DPlotWidget* data3DPlotWidget_;

};
