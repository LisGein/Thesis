#pragma once
#include <QTableWidget>


class Dataset;

class DatasetAnalysisTable: public QTableWidget
{
	Q_OBJECT
public:
    DatasetAnalysisTable(QWidget* parent = 0);
    virtual ~DatasetAnalysisTable();
    void setFeatures(const QStringList &features, const Dataset &dataset);


public slots:

signals:

private:
    QStringList features_;

};
