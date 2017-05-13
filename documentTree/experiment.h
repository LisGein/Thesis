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

	/// \brief set column ID of response
	/// \param response ID in scope of filtered dataset
	void setResponse(int response);

	/// \brief get response column
	/// \return ID in scope of filtered dataset
	int getResponseColumn() const { return response_; }

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
