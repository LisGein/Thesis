#pragma once
#include <QWidget>
#include <memory>


class Dataset;
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

	void setDataset(Dataset* dataset);

private:
	std::unique_ptr<Ui::ExperimentWidget> ui_;
	Dataset* dataset_;
};
