#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
using namespace std;

#include "xml_tag.h"
#include "xml_content.h"

#ifndef XML_TREE
#define XML_TREE

class XML_TreeNode {
public:
	XML_TreeNode();
	~XML_TreeNode();


	//The types of tree node
	enum {
		TAG_NODE,
		CONTENT_NODE
	};

private:
	//The data for if this is a tag node
	XML_Tag* start_tag;
	XML_Tag* end_tag;

	//The data for if this is a content node
	string content_string;

	//The tree structure
	XML_TreeNode* parent;
	vector<XML_TreeNode*> children;
};

#endif
