#include "xml_tree.h"

XML_TreeNode::XML_TreeNode() {
	//The data for if this is a tag node
	start_tag = NULL;
	end_tag = NULL;

	//The content of this node
	node_content.clear();

	//The tree structure
	parent = NULL;
	children.clear();

	next_sibling = NULL;
	prev_sibling = NULL;
}

XML_TreeNode::~XML_TreeNode() {
	//Do nothing
}

//////////////////////////////////
// Content management functions //
//////////////////////////////////

//Modify the content
int XML_TreeNode::SetNodeContent(vector<XML_Content> node_content) {
	node_content = node_content;
	return true;
}

int XML_TreeNode::AddNodeContent(XML_Content new_content) {
	node_content.push_back(new_content);
	return true;
}

unsigned int XML_TreeNode::GetNodeContentCount() {
	return node_content.size();
}

XML_Content XML_TreeNode::GetNodeContent(unsigned int index) {
	return node_content[index];
}

//Recursively parse the content to create a subtree
int XML_TreeNode::ParseContent() {

	return true;
}

///////////////////////////////////
// Tree search related functions //
///////////////////////////////////

//////////////////////////////////////
// Tree structure related functions //
//////////////////////////////////////

//Modify the parent/child nodes
int XML_TreeNode::SetParentNode(XML_TreeNode* parent) {
	this->parent = parent;
	return true;
}

XML_TreeNode* XML_TreeNode::GetParentNode() {
	return parent;
}

int XML_TreeNode::SetChildrenNodes(vector<XML_TreeNode*> children) {
	this->children = children;
	return true;
}

int XML_TreeNode::AddChildNode(XML_TreeNode* child) {
	children.push_back(child);
	return true;
}

unsigned int XML_TreeNode::GetChildrenNodeCount() {
	return children.size();
}

XML_TreeNode* XML_TreeNode::GetChildNode(unsigned int index) {
	if(index >= children.size())
		return NULL;

	return children[index];
}

//Modify the siblings
int XML_TreeNode::SetNextSibling(XML_TreeNode* next_sibling) {
	this->next_sibling = next_sibling;
	return true;
}

XML_TreeNode* XML_TreeNode::GetNextSibling() {
	return next_sibling;
}

int XML_TreeNode::SetPrevSibling(XML_TreeNode* prev_sibling) {
	this->prev_sibling = prev_sibling;
	return true;
}

XML_TreeNode* XML_TreeNode::GetPrevSibling() {
	return prev_sibling;
}
