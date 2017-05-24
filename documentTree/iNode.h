#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>


class INode
{
public:
	enum class TypeObject
	{
		Invalid = -1,
		Node = 0,
		Document = 1,
		Experiment = 2,
		Regression = 3
	};

	INode(const std::string& name, const std::string& childCategoryName);
	virtual ~INode() = default;

	virtual int childCount() const = 0;
	virtual INode* child(int id) = 0;
	virtual const INode* parentItem() const = 0;
	virtual void addNewChild() = 0;

    virtual void openRegression(boost::property_tree::ptree &inventoryTree) = 0;
    virtual void saveRegression(boost::property_tree::ptree &inventoryTree) = 0;

	const INode* child(int id) const;

	virtual TypeObject type() const;
	virtual const std::string& displayName() const;
	virtual const std::string& childCategoryName() const;
private:

	std::string name_;
	std::string childCategoryName_;
};

class Root : public INode
{
public:
	Root(INode& firstLevel);

	// INode interface
public:
	int childCount() const override;
	INode* child(int) override;
	const INode* parentItem() const override;
	void addNewChild() override;

    virtual void openRegression(boost::property_tree::ptree &inventoryTree);
    virtual void saveRegression(boost::property_tree::ptree &inventoryTree);


private:
	INode& firstLevel_;
};
