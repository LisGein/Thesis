#pragma once
#include <QWidget>
#include <memory>


class Experiment;
class ExperimentTable;
class DatasetAnalysisTable;
class FeatureFilterModel;

namespace Ui
{
class ExperimentWidget;
}

class ExperimentWidget : public QWidget
{
	Q_OBJECT
public:
	ExperimentWidget(QWidget* parent = 0);
	virtual ~ExperimentWidget();

	void setExperiment(Experiment* experiment);

public slots:
	void updateResponse(const QString &name);

private:
	std::unique_ptr<Ui::ExperimentWidget> ui_;
	std::unique_ptr<FeatureFilterModel> featureFilterModel_;
	Experiment* experiment_;
};
