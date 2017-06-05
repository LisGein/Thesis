#pragma once
#include <memory>
#include <vector>
#include "iNode.h"


class Dataset;
class DatasetModel;
class Experiment;

class Document : public INode
{
public:
	Document();
	virtual ~Document();

	DatasetModel& getDatasetModel();
	Dataset& getDataset();
	const Dataset& getDataset() const;


	void loadFromTsv(const std::string& tsv);

	virtual void openRegression(boost::property_tree::ptree &inventoryTree) override;
	virtual void saveRegression(boost::property_tree::ptree &inventoryTree) override;

	// INode interface
public:
	virtual int childCount() const override;
	virtual INode* child(int id) override;
	virtual const INode* parentItem() const override;
	virtual void addNewChild() override;
	virtual void removeAllChilds() override;
	virtual TypeObject type() const override;

private:
	std::unique_ptr<Dataset> dataset_;
	std::unique_ptr<DatasetModel> datasetModel_;
	
	std::vector<std::unique_ptr<Experiment>> experiments_;

};
