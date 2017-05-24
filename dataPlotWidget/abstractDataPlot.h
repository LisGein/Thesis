#pragma once

#include <QWidget>

#include <memory>
#include <set>

#include "mlpack/core.hpp"


class LinearRegressionModel;
class QComboBox;
class QLabel;

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
    virtual void setAxisNames(const std::set<int> &axisIds);

public slots:
	void updateRegression();

protected:
    virtual void updateChart(const arma::mat& data, const arma::vec& resp) = 0;
    std::pair<double, double> bounds(const arma::mat &data, const arma::vec &column) const;
    virtual void clear() = 0;


    LinearRegressionModel* linearRegression_;
    std::set<int> axisIds_;

    std::unique_ptr<DataPlot::ComboBox> axisXCombo_;
    std::unique_ptr<DataPlot::ComboBox> axisZCombo_;

    static const int GRID_SIZE;
};
