#pragma once
#include <QMainWindow>
#include <memory>

class QSettings;

class MainWindow : public QMainWindow {
	 Q_OBJECT

public:
	 MainWindow(QWidget *parent = 0);
	 virtual ~MainWindow();

private:
	 void initMenu();

private:
	 QWidget *mainworkplace_;


	 std::unique_ptr<QSettings> appSettings_;
};
