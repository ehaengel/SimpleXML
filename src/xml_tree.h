#include <stdlib.h>
#include <string.h>
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

	////////////////////////
	// File i/o functions //
	////////////////////////

	int WriteToFile(FILE* handle);

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

	//Count all the tags on this node of a given tag name
	unsigned int CountTagsOfTagName(string tag_name);

	//Get all the tags on this node of given tag name
	int GetTagsOfTagName(string tag_name, vector<XML_Tag*> &result);

	//Get all the children of this node of tag type and given tag name
	int GetTreeNodesOfTagName(string tag_name, vector<XML_TreeNode*> &result);

	//////////////////////////////////////
	// Tree structure related functions //
	//////////////////////////////////////

	//Create a new child node which is a pair of tags or a single open-close tag
	XML_TreeNode* CreateChildTagPair(string tag_name);
	XML_TreeNode* CreateChildTagOpenClosed(string tag_name);

	//Create a new child from some content
	XML_TreeNode* CreateChildFromTag(XML_Tag* start_tag, XML_Tag* end_tag);
	XML_TreeNode* CreateChildFromString(string content_string);

	//Modify the start/end tag
	int SetStartTag(XML_Tag* start_tag);
	XML_Tag* GetStartTag();

	int SetEndTag(XML_Tag* end_tag);
	XML_Tag* GetEndTag();

	//Modify the parent/child nodes
	int SetParentNode(XML_TreeNode* parent);
	XML_TreeNode* GetParentNode();

	int SetChildrenNodes(vector<XML_TreeNode*> children);
	int AddChildNode(XML_TreeNode* child);

	unsigned int GetChildrenNodeCount();
	XML_TreeNode* GetChildNode(unsigned int index);

	//Modify the siblings
	int SetNextSibling(XML_TreeNode* next_sibling);
	XML_TreeNode* GetNextSibling();

	int SetPrevSibling(XML_TreeNode* prev_sibling);
	XML_TreeNode* GetPrevSibling();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int WriteDotFile(const char* filename);
	int WriteDotFile(FILE* handle);

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
