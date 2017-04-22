#pragma once
#include <QWidget>
#include <QMap>
#include <memory>

class FeatureGraphicsScene;
class QLineEdit;
class DatasetView;

class Document;

class MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = 0);
	virtual ~MainWidget();

private slots:
	void onInsertTable(QString str);
	void deleteFromFormula(QPair<int, int> feature);
	void addToFormula(QPair<int, int> feature);

private:
	void updateRegression();
	void updateFormulaText();
	void onDatasetUpdated();

	QLineEdit* formulaEdit_;
	QMap<QString, QString> lineText_;
	QString startFormula_;

	DatasetView *datasetView_;
	QMultiMap<QString, QString> addedParents_;

	std::unique_ptr<Document> document_;
};
