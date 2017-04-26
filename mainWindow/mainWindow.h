#pragma once
#include <QMainWindow>
#include <memory>

namespace Ui
{
	class MainWindow;
}
class Root;
class TreeModel;
class Document;
class QSettings;
class DatasetWidget;
class ViewController;
class ExperimentWidget;
class INode;

class MainWindow : public QMainWindow {
	 Q_OBJECT

public:
	 MainWindow(QWidget *parent = 0);
	 virtual ~MainWindow();

private slots:
	 void changeCurrentWidget(INode* node);
	void insertTable(QString str);

private:
	 void initMenu();

private:
	 std::unique_ptr<QSettings> appSettings_;

	 std::unique_ptr<Document> document_;
	 std::unique_ptr<Root> fakeRoot_;
	 std::unique_ptr<TreeModel> documentModel_;

	 std::unique_ptr<DatasetWidget> datasetWidget_;
	 std::unique_ptr<ExperimentWidget> experimentWidget_;
	 std::unique_ptr<ViewController> graphicsController_;

	 std::unique_ptr<Ui::MainWindow> ui_;
};
