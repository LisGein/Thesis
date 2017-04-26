#pragma once
#include <QTreeView>

class TreeModel;
class INode;

class TreeView : public QTreeView
{
	Q_OBJECT
public:
	TreeView(QWidget *parent = nullptr);
	virtual ~TreeView();

	void setModel(TreeModel *model);
	void contextMenuEvent(QContextMenuEvent *event) override;


protected slots:
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private slots:
	void onAddNewChild();

signals:
	void childAdded(const QModelIndex &parent);
	void changedCurrentWidget(INode* node);

private:
	QModelIndex clickedIndex_;
	TreeModel *model_;
};
