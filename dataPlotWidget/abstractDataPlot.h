#pragma once

#include <QWidget>

#include <memory>
#include <set>

#include <armadillo>


class LinearRegressionModel;
class QComboBox;
class QLabel;
class QSplitter;
class ChartSettings;

namespace DataPlot {


class ComboBox
{
public:
	ComboBox(const QString &text);
	~ComboBox();
	void hide();

	QComboBox* box;
	QLabel* label;
};
}

class AbstractDataPlot : public QWidget
{
	Q_OBJECT
public:
	AbstractDataPlot(QWidget *parent = nullptr);
	virtual ~AbstractDataPlot();

	virtual void setRegression(LinearRegressionModel* linearRegression);
	virtual void setAxisNames(const std::map<int, std::pair<double, double> > &axisIds);

public slots:
	void updateRegression();

protected:
	virtual void updateChart(const arma::mat& data, const arma::vec& resp) = 0;
	std::pair<double, double> bounds(const arma::mat &data, const arma::vec &column) const;
	virtual void clear() = 0;


	LinearRegressionModel* linearRegression_;
	std::map<int, std::pair<double, double> > axisIds_;

	std::unique_ptr<ChartSettings> chartSettings_;
	std::unique_ptr<DataPlot::ComboBox> axisXCombo_;
	std::unique_ptr<DataPlot::ComboBox> axisZCombo_;
	std::unique_ptr<DataPlot::ComboBox> ZCombo_;
	QSplitter *splitter_;

	static const int GRID_SIZE;
};
