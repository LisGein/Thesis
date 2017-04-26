#pragma once
#include "iNode.h"

#include "dataset.h"

#include <memory>
#include <vector>
#include <set>

class Regression;
class Document;


class Experiment : public INode
{
public:
	Experiment(const Document& document);
	virtual ~Experiment();

	const Dataset& getDataset() const;
	const Dataset& getFiltredDataset() const;
	const std::set<int>& getEnabledFeatures() const;
	void setEnabledFeatures(const std::set<int>& enabled);
	void setResponse(int response);

	void update();

	// INode interface
public:
	int childCount() const override;
	INode* child(int id) override;
	const INode* parentItem() const override;
	void addNewChild() override;
	virtual TypeObject type() const override;

private:
	void updateFiltredDataset();

private:
	std::vector<std::unique_ptr<Regression>> regressions_;
	const Document& document_;
	Dataset filtredDataset_;
	std::set<int> enabledFeatures_;
	int response_;
};
