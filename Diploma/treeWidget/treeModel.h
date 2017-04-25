#pragma once
#include <QAbstractTableModel>

class INode;

class TreeModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	//QAbstractTableModel interface
	explicit TreeModel(INode* root, QObject *parent = 0);
	virtual ~TreeModel();

	QVariant data(const QModelIndex &index, int role) const override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	QVariant headerData(int section, Qt::Orientation orientation,
							  int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
							const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	int typeObject(const QModelIndex &index);

public slots:
	void onAddNewChild(const QModelIndex& parent);


private:
	INode* getNodeFromIndexSafe(const QModelIndex& index) const;
	int findMyRowId(const INode* node) const;

private:
	INode *rootItem_;
};
