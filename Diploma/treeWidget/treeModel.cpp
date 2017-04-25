#include "common/common.h"

#include "treeModel.h"
#include "../documentTree/iNode.h"


TreeModel::TreeModel(INode* root, QObject* parent)
	: QAbstractTableModel(parent)
	, rootItem_(root)
{
}

TreeModel::~TreeModel()
{
}


QVariant TreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		 return QVariant();

	INode *node = getNodeFromIndexSafe(index);

	return to_qt(node->displayName() + " " + std::to_string(index.row()));
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		 return 0;

	return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal && section == 0 && role == Qt::DisplayRole)
		return "Name";

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	INode *parentNode = getNodeFromIndexSafe(parent);

	INode *childItem = parentNode->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

int TreeModel::findMyRowId(const INode *node) const
{
	int rowInParentScope = 0;
	if (node)
	{
		const auto* parent = node->parentItem();
		if (parent)
		{
			for (int i = 0; i < parent->childCount(); ++i)
			{
				if (parent->child(i) == node)
					rowInParentScope = i;
			}
		}
	}

	return rowInParentScope;
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
	INode* node = getNodeFromIndexSafe(index);
	if (node == rootItem_)
		return QModelIndex();

	 const INode* parent = node->parentItem();

	return createIndex(findMyRowId(parent), 0, const_cast<INode *>(parent));
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
	INode* parentNode;
	if (parent.column() > 0)
		return 0;

	parentNode = getNodeFromIndexSafe(parent);

	return parentNode->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return 1;
}

int TreeModel::typeObject(const QModelIndex& index)
{
	if (!index.isValid() || !index.internalPointer())
		return -1;

	INode *node = static_cast<INode*>(index.internalPointer());
	return int(node->type());
}

INode* TreeModel::getNodeFromIndexSafe(const QModelIndex& index) const
{
	INode* node;
	if (!index.isValid() || !index.internalPointer())
		node = rootItem_;
	else
		node = static_cast<INode*>(index.internalPointer());


	return node;
}

void TreeModel::onAddNewChild(const QModelIndex& parent)
{
	int rowId = rowCount(parent);
	beginInsertRows(parent, rowId, rowId);

	INode* node = getNodeFromIndexSafe(parent);
	node->addNewChild();

	endInsertRows();
}



