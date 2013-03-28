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
	this->node_content = node_content;
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
	//Clear the children
	children.clear();

	//printf("Number of content elements: %u\n", node_content.size());

	//Run through the content list
	unsigned int index = 0;
	while(index < node_content.size()) {
		//This is a tag
		if(node_content[index].GetContentTag() != NULL) {
			XML_Tag* content_tag = node_content[index].GetContentTag();

			//printf("content tag: %s\n", content_tag->GetTagName().c_str());

			//This is an open-close tag
			if(content_tag->GetTagType() == XML_Tag::OPENCLOSE_TAG) {
				XML_TreeNode* new_child = new XML_TreeNode;

				new_child->SetStartTag(content_tag);
				new_child->SetEndTag(content_tag);
				new_child->SetParentNode(this);

				AddChildNode(new_child);
			}

			//This is a random close tag, which is an error
			else if(content_tag->GetTagType() == XML_Tag::CLOSE_TAG)
				return false;

			//This is an open tag
			else if(content_tag->GetTagType() == XML_Tag::OPEN_TAG) {
				//Look for the closing tag
				string tag_name = content_tag->GetTagName();
				int open_close_count = 1;

				index++;

				XML_Tag* content_tag_end = NULL;
				vector<XML_Content> internal_content;
				while(index < node_content.size()) {
					if(node_content[index].GetContentTag() != NULL) {
						XML_Tag* cur_tag = node_content[index].GetContentTag();

						if(strcmp(cur_tag->GetTagName().c_str(), tag_name.c_str()) == 0) {
							if(cur_tag->GetTagType() == XML_Tag::OPEN_TAG)
								open_close_count++;

							if(cur_tag->GetTagType() == XML_Tag::CLOSE_TAG)
								open_close_count--;
						}

						if(open_close_count == 0) {
							content_tag_end = cur_tag;
							break;
						}
					}

					if(open_close_count != 0)
						internal_content.push_back(node_content[index]);

					index++;
				}

				//This is not a properly formated XML document
				if(open_close_count != 0)
					return false;

				//Create a new child node
				XML_TreeNode* new_child = new XML_TreeNode;

				//printf("Internal content count: %u\n", internal_content.size());
				//printf("End tag name: %s\n", content_tag_end->GetTagName().c_str());

				new_child->SetNodeContent(internal_content);
				new_child->SetParentNode(this);

				new_child->SetStartTag(content_tag);
				new_child->SetEndTag(content_tag_end);

				AddChildNode(new_child);
			}
		}

		//This is a string
		else {
			XML_TreeNode* new_child = new XML_TreeNode;

			new_child->AddNodeContent(node_content[index]);
			new_child->SetParentNode(this);

			AddChildNode(new_child);
		}

		index++;
	}

	//Set the siblings for all the children
	if(children.size() > 1) {
		for(unsigned int i=0; i<children.size()-1; i++)
			children[i]->SetNextSibling(children[i+1]);

		for(unsigned int i=1; i<children.size(); i++)
			children[i]->SetPrevSibling(children[i-1]);
	}

	//Have the children parse their own content
	for(unsigned int i=0; i<children.size(); i++) {
		int run_parser = true;

		if(children[i]->GetStartTag() == NULL)
			run_parser = false;

		if(run_parser && children[i]->ParseContent() == false)
			return false;
	}

	return true;
}

///////////////////////////////////
// Tree search related functions //
///////////////////////////////////

//////////////////////////////////////
// Tree structure related functions //
//////////////////////////////////////

//Modify the start/end tag
int XML_TreeNode::SetStartTag(XML_Tag* start_tag) {
	this->start_tag = start_tag;
	return true;
}

XML_Tag* XML_TreeNode::GetStartTag() {
	return start_tag;
}

int XML_TreeNode::SetEndTag(XML_Tag* end_tag) {
	this->end_tag = end_tag;
	return true;
}

XML_Tag* XML_TreeNode::GetEndTag() {
	return end_tag;
}

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

/////////////////////////
// Debugging functions //
/////////////////////////

int XML_TreeNode::WriteDotFile(const char* filename) {
	FILE* handle = fopen(filename, "w");
	if(!handle)
		return false;

	fprintf(handle, "graph G {\n");

	if(WriteDotFile(handle) == false) {
		fclose(handle);
		return false;
	}

	fprintf(handle, "}\n");
	fclose(handle);
	return true;
}

int XML_TreeNode::WriteDotFile(FILE* handle) {
	string label = "blank";
	if(start_tag != NULL && end_tag != NULL) {
		if(start_tag == end_tag)
			label = "<" + start_tag->GetTagName() + ">";

		else
			label = "<" + start_tag->GetTagName() + ">..</" + end_tag->GetTagName() + ">";
	}

	else
		label = "Content";

	fprintf(handle, "\t%u [label = \"%s\"]\n", (unsigned int)this, label.c_str());

	for(unsigned int i=0; i<children.size(); i++)
		fprintf(handle, "\t%u -- %u\n", (unsigned int)this, (unsigned int)children[i]);

	fprintf(handle, "\n");

	for(unsigned int i=0; i<children.size(); i++)
		if(children[i]->WriteDotFile(handle) == false)
			return false;

	return true;
}
