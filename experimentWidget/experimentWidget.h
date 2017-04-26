#pragma once
#include <QWidget>
#include <memory>


class Experiment;
class ExperimentTable;

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
	void updateFiltredDataset();


private:
	std::unique_ptr<Ui::ExperimentWidget> ui_;
	Experiment* experiment_;
};
