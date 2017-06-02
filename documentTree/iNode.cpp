#include "iNode.h"


INode::INode(const std::string& name, const std::string& childCategoryName)
	: name_(name)
	, childCategoryName_(childCategoryName)
{

}

const INode*INode::child(int id) const
{
	return const_cast<INode *>(this)->child(id); //common hack
}

INode::TypeObject INode::type() const
{
	return TypeObject::Node;
}

const std::string& INode::displayName() const
{
	return name_;
}

const std::string& INode::childCategoryName() const
{
	return childCategoryName_;
}


Root::Root(INode& firstLevel)
	: INode ("", "Document")
	, firstLevel_(firstLevel)
{
}

int Root::childCount() const
{
	return 1;
}

INode* Root::child(int)
{
	return &firstLevel_;
}

const INode* Root::parentItem() const
{
	return nullptr;
}

void Root::addNewChild()
{
	return;
}

void Root::openRegression(boost::property_tree::ptree &inventoryTree)
{

}

void Root::saveRegression(boost::property_tree::ptree &inventoryTree)
{

}

