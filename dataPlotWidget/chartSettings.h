#pragma once
#include <QWidget>
#include <memory>


namespace Ui
{
class ChartSettings;
}
class FeatureModel;

class ChartSettings: public QWidget
{
	Q_OBJECT
public:
	ChartSettings(QWidget *parent = 0);
	virtual ~ChartSettings();
	void hideYBox();
	void clearAxis();
	void setAxisNames(const std::map<int, std::pair<double, double> > &axisIds);
	void setModel(const FeatureModel *model);

	std::unique_ptr<Ui::ChartSettings> ui;

private slots:
	void updateRange();
	void updateOtherAxisNames();

private:
	void updateAxis();

	const FeatureModel *model_;
	std::map<int, std::pair<double, double> > axisIds_;
};
