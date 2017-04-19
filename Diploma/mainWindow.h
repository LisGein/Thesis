#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
	 Q_OBJECT

public:
	 MainWindow(QWidget *parent = 0);
	 virtual ~MainWindow();

private:
	 QWidget *mainworkplace_;

	 void initMenu();
};
