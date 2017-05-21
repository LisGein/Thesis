#pragma once

#include <QWidget>
#include <memory>
#include <set>


class LinearRegressionModel;

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
	virtual void updateChart() = 0;


    LinearRegressionModel* linearRegression_;
    std::set<int> axisIds_;
};
