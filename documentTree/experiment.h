#pragma once
#include "iNode.h"
#include "document.h"
#include <memory>
#include <vector>

class Regression;
//class Document;
class Dataset;


class Experiment : public INode
{
public:
	Experiment(const Document& document);
	virtual ~Experiment();

	const Dataset& getDataset() const;

	void update();

	// INode interface
public:
	int childCount() const override;
	INode* child(int id) override;
	const INode* parentItem() const override;
	void addNewChild() override;
	virtual TypeObject type() const override;

private:
	std::vector<std::unique_ptr<Regression>> regressions_;
	const Document& document_;

};
