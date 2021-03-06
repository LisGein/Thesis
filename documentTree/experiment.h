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

	virtual void openRegression(boost::property_tree::ptree &inventoryTree) override;
	virtual void saveRegression(boost::property_tree::ptree &experimentTree) override;


	/// \brief set column ID of response
	/// \param response ID in scope of origin dataset
	void setResponse(int response);

	/// \brief get response column
	/// \return ID in scope of origin dataset
	int getResponseColumn() const { return response_.originColumns(0); }

	void addFeature (int feature);
	void removeFeature (int feature);

	void update();

	// INode interface
public:
	virtual int childCount() const override;
	virtual INode* child(int id) override;
	virtual const INode* parentItem() const override;
	virtual void addNewChild() override;
	virtual void removeAllChilds() override;
	virtual TypeObject type() const override;


private:
	std::vector<std::unique_ptr<Regression>> regressions_;
	const Document& document_;
	DatasetColumnsView filtredDataset_;
	DatasetColumnsView response_;
};
