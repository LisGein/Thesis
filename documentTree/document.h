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

	// INode interface
public:
	int childCount() const override;
	INode* child(int id) override;
	const INode* parentItem() const override;
	void addNewChild() override;
	virtual TypeObject type() const override;

private:
	std::unique_ptr<Dataset> dataset_;
	std::unique_ptr<DatasetModel> datasetModel_;
	
	std::vector<std::unique_ptr<Experiment>> experiments_;

};
