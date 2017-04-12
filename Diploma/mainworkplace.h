#pragma once
#include <QWidget>

class QTreeView;
class QTableWidget;

class MainWorkPlace : public QWidget
{
	 Q_OBJECT

public:
	 MainWorkPlace(QWidget *parent = 0);
	 virtual ~MainWorkPlace();

private:
	 QTreeView *treeview_;
	 QTableWidget *table_;

};
