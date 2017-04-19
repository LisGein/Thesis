#pragma once
#include <QTableWidget>

#include <QShortcut>

class TableWidget : public QTableWidget
{
	Q_OBJECT

public:
	TableWidget(QWidget *parent = 0);
	virtual ~TableWidget();

	virtual void contextMenuEvent(QContextMenuEvent *event) override;

signals:
	void insertedTable(QStringList params);

private slots:
	void pasteTable();

private:
	const QStringList parseTable(const QString& str);

	QShortcut *insert_shortcut_;
};
