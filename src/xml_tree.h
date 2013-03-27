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

	//////////////////////////////////
	// Content management functions //
	//////////////////////////////////

	//Modify the content
	int SetNodeContent(vector<XML_Content> node_content);
	int AddNodeContent(XML_Content new_content);

	unsigned int GetNodeContentCount();
	XML_Content GetNodeContent(unsigned int index);

	//Recursively parse the content to create a subtree
	int ParseContent();

	///////////////////////////////////
	// Tree search related functions //
	///////////////////////////////////

	//////////////////////////////////////
	// Tree structure related functions //
	//////////////////////////////////////

	//Modify the start/end tag
	int SetStartTag(XML_Tag* start_tag);
	XML_Tag* GetStartTag();

	int SetEndTag(XML_Tag* end_tag);
	XML_Tag* GetEndTag();

	//Modify the parent/child tags
	int SetParentTag(XML_Tag* parent_tag);
	XML_Tag* GetParentTag();

	int SetChildrenTags(vector<XML_Tag*> children_tags);
	int AddChildTag(XML_Tag* child_tag);

	unsigned int GetChildrenTagCount();
	XML_Tag* GetChildTag(unsigned int index);

	//Modify the siblings
	int SetNextSibling(XML_Tag* next_sibling_tag);
	XML_Tag* GetNextSibling();

	int SetPrevSibling(XML_Tag* prev_sibling_tag);
	XML_Tag* GetPrevSibling();

private:
	//The data for if this is a tag node
	XML_Tag* start_tag;
	XML_Tag* end_tag;

	//The content of this node
	vector<XML_Content> node_content;

	//The tree structure
	XML_TreeNode* parent;
	vector<XML_TreeNode*> children;

	XML_TreeNode* next_sibling;
	XML_TreeNode* prev_sibling;
};

#endif
