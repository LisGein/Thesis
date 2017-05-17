#pragma once
#include "iNode.h"

#include "dataset.h"
#include "datasetColumnsView.h"

#include <memory>
#include <vector>
#include <set>
#include <list>


class Regression;
class Document;

class Experiment : public INode
{
public:
	Experiment(const Document& document);
	virtual ~Experiment();

    const Dataset& getDataset() const;

    const DatasetColumnsView& filtredDataset() const;
    const DatasetColumnsView& responses() const;


    const std::vector<int> getEnabledFeatures() const;
    void setEnabledFeatures(const std::list<int> &enabled);

	/// \brief set column ID of response
	/// \param response ID in scope of filtered dataset
	void setResponse(int response);

	/// \brief get response column
	/// \return ID in scope of filtered dataset
    int getResponseColumn() const { return response_.originColumns(0); }

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
    DatasetColumnsView filtredDataset_;
    DatasetColumnsView response_;
};
