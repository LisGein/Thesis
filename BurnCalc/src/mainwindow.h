#pragma once

#include <QMainWindow>
#include <QScopedPointer>

class GuiEmpericalFormula;

namespace Ui
{
    class MainWindow;
}

class CMainWindow : public QMainWindow {
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = 0);
    virtual ~CMainWindow();

private:
    QScopedPointer<Ui::MainWindow> ui;

    GuiEmpericalFormula *substances;
};
