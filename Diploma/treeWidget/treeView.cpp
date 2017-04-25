#include "common/common.h"
#include "treeView.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "treeModel.h"
#include "documentTree/iNode.h"


TreeView::TreeView(QWidget* parent)
	: QTreeView(parent)
	, model_(nullptr)
{
}

TreeView::~TreeView()
{

}

void TreeView::setModel(TreeModel* model)
{
	model_ = model;
	QTreeView::setModel(model);
}

void TreeView::contextMenuEvent(QContextMenuEvent* event)
{
	QModelIndex index = indexAt(event->pos());

	if (!index.isValid())
		return;

	clickedIndex_ = index;

	auto* node = static_cast<INode*>(index.internalPointer());
	if (!node || node->childCategoryName().empty())
		return;

	QAction *act = new QAction(to_qt("New " + node->childCategoryName()));
	QObject::connect(act, SIGNAL(triggered(bool)), this, SLOT(onAddNewChild()));

	QMenu menu(this);
	menu.addAction(act);
	menu.exec(event->globalPos());

	QObject::disconnect(act, SIGNAL(triggered(bool)), this, SLOT(onAddNewChild()));
}


void TreeView::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	int type = (model_) ? model_->typeObject(current) : -1;
	emit changedCurrentWidget(type);
	QTreeView::currentChanged(current, previous);
}

void TreeView::onAddNewChild()
{
	emit childAdded(clickedIndex_);
	expand(clickedIndex_);
}
