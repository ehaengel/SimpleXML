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

