#pragma once
#include <QWidget>
#include <QMap>

class featureGraphicsScene;
class QLineEdit;
class TableView;

class MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = 0);
	virtual ~MainWidget();

private slots:
	void convertInsertedTable(QStringList list);
	void deleteFromFormula(QPair<QString, QString> parents);
	void addToFormula(QPair<QString, QString> parents);

private:
	QString generateTextForFormula(const QPair<QString, QString> &parents) const;
	QString generateSymbForFormula() const;
	void updateRegression();


	featureGraphicsScene* scene_;
	QLineEdit* line_;
	QMap<QString, QString> lineText_;
	QString startFormula_;

	TableView *table_;
	QMultiMap<QString, QString> addedParents_;
};
