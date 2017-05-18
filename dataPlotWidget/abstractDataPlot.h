#pragma once

#include <QWidget>
#include <memory>


class LinearRegressionModel;

class AbstractDataPlot : public QWidget
{
	Q_OBJECT
public:
    AbstractDataPlot(QWidget *parent = nullptr);
	virtual ~AbstractDataPlot();

    virtual void setRegression(LinearRegressionModel* linearRegression);
    virtual void setAxisNames(const std::vector<std::string> &names) = 0;

public slots:
	void updateRegression();

protected:
	virtual void updateChart() = 0;


    LinearRegressionModel* linearRegression_;
};
