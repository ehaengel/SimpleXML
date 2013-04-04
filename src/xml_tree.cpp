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
	//Delete all the children and clear the content
	node_content.clear();

	for(unsigned int i=0; i<GetChildrenNodeCount(); i++) {
		if(GetChildNode(i) != NULL)
			delete GetChildNode(i);
	}
}

////////////////////////
// File i/o functions //
////////////////////////

int XML_TreeNode::WriteToFile(FILE* handle) {
	//First figure out the depth of this tree node
	unsigned int node_depth = 0;
	XML_TreeNode* temp_node = GetParentNode();

	while(temp_node != NULL) {
		node_depth++;
		temp_node = temp_node->GetParentNode();
	}

	//If this is a leaf node
	if(GetChildrenNodeCount() == 0) {
		//If this leaf node is just an empty open/close tag pair
		if(GetStartTag() != GetEndTag() && GetStartTag() != NULL && GetEndTag() != NULL) {
			//Create some tabbing
			for(unsigned int i=1; i<node_depth; i++)
				fprintf(handle, "\t");

			//Print the open tag
			if(GetStartTag()->WriteToFile(handle) == false)
				return false;

			//Create some tabbing
			for(unsigned int i=1; i<node_depth; i++)
				fprintf(handle, "\t");

			//Print the end tag
			if(GetEndTag()->WriteToFile(handle) == false)
				return false;
		}

		//If this is a open-close tag leaf node
		else if(GetStartTag() == GetEndTag() && GetStartTag() != NULL) {
			XML_Tag* cur_tag = GetStartTag();

			//Create some tabbing
			for(unsigned int i=1; i<node_depth; i++)
				fprintf(handle, "\t");

			if(cur_tag->WriteToFile(handle) == false)
				return false;
		}

		//If this is a content leaf node
		else {
			for(unsigned int i=0; i<GetNodeContentCount(); i++) {
				XML_Content cur_content = GetNodeContent(i);

				//Create some tabbing
				for(unsigned int i=1; i<node_depth; i++)
					fprintf(handle, "\t");

				fprintf(handle, "%s\n", cur_content.GetContentString().c_str());
			}
		}
	}

	//This node represents an open/close tag pair
	else {
		if(GetStartTag() != NULL) {
			//Create some tabbing
			for(unsigned int i=1; i<node_depth; i++)
				fprintf(handle, "\t");

			//Print the open tag
			if(GetStartTag()->WriteToFile(handle) == false)
				return false;
		}

		//Recursively write the children tags
		for(unsigned int i=0; i<GetChildrenNodeCount(); i++) {
			XML_TreeNode* cur_child = GetChildNode(i);

			if(cur_child->WriteToFile(handle) == false)
				return false;
		}

		if(GetEndTag() != NULL) {
			//Create some tabbing
			for(unsigned int i=1; i<node_depth; i++)
				fprintf(handle, "\t");

			//Print the close tag
			if(GetEndTag()->WriteToFile(handle) == false)
				return false;
		}
	}

	return true;
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
	for(unsigned int i=0; i<children.size(); i++)
		if(children[i] != NULL)
			delete children[i];

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

//Count all the tags on this node of a given tag name
unsigned int XML_TreeNode::CountTagsOfTagName(string tag_name) {
	unsigned int count = 0;

	for(unsigned int i=0; i<GetChildrenNodeCount(); i++) {
		XML_TreeNode* cur_child = GetChildNode(i);

		if(cur_child != NULL) {
			if(cur_child->GetStartTag() != NULL) {
				XML_Tag* cur_tag = cur_child->GetStartTag();

				if(strcmp(cur_tag->GetTagName().c_str(), tag_name.c_str()) == 0)
					count++;
			}
		}
	}

	return count;
}

//Get all the tags on this node of given tag name
int XML_TreeNode::GetTagsOfTagName(string tag_name, vector<XML_Tag*> &result) {
	result.clear();
	for(unsigned int i=0; i<GetChildrenNodeCount(); i++) {
		XML_TreeNode* cur_child = GetChildNode(i);

		if(cur_child != NULL) {
			if(cur_child->GetStartTag() != NULL) {
				XML_Tag* cur_tag = cur_child->GetStartTag();

				if(strcmp(cur_tag->GetTagName().c_str(), tag_name.c_str()) == 0)
					result.push_back(cur_tag);
			}
		}
	}

	return true;
}

//Get all the children of this node of tag type and given tag name
int XML_TreeNode::GetTreeNodesOfTagName(string tag_name, vector<XML_TreeNode*> &result) {
	result.clear();

	for(unsigned int i=0; i<GetChildrenNodeCount(); i++) {
		XML_TreeNode* cur_child = GetChildNode(i);

		if(cur_child != NULL) {
			if(cur_child->GetStartTag() != NULL) {
				XML_Tag* cur_tag = cur_child->GetStartTag();

				if(strcmp(cur_tag->GetTagName().c_str(), tag_name.c_str()) == 0)
					result.push_back(cur_child);
			}
		}
	}

	return true;
}

//////////////////////////////////////
// Tree structure related functions //
//////////////////////////////////////

/*XML_TreeNode* XML_TreeNode::CreateChildFromContent(vector<XML_Content> content_items) {
	XML_TreeNode* new_child = new XML_TreeNode;

	new_child->SetParentNode(this);
	new_child->SetNodeContent(content_items);

	if(new_child->ParseContent() == true)
		AddChildNode(new_child);

	else {
		delete new_child;
		new_child = NULL;
	}

	return new_child;
}*/

//Create a new child node which is a pair of tags or a single open-close tag
XML_TreeNode* XML_TreeNode::CreateChildTagPair(string tag_name) {
	XML_Tag* new_start_tag = new XML_Tag;
	new_start_tag->SetTagName(tag_name);
	new_start_tag->SetTagType(XML_Tag::OPEN_TAG);

	XML_Tag* new_end_tag = new XML_Tag;
	new_end_tag->SetTagName(tag_name);
	new_end_tag->SetTagType(XML_Tag::CLOSE_TAG);

	return CreateChildFromTag(new_start_tag, new_end_tag);
}

XML_TreeNode* XML_TreeNode::CreateChildTagOpenClosed(string tag_name) {
	XML_Tag* new_tag = new XML_Tag;

	new_tag->SetTagName(tag_name);
	new_tag->SetTagType(XML_Tag::OPENCLOSE_TAG);

	return CreateChildFromTag(new_tag, new_tag);
}

//Create a new child from some content
XML_TreeNode* XML_TreeNode::CreateChildFromTag(XML_Tag* start_tag, XML_Tag* end_tag) {
	XML_TreeNode* new_child = new XML_TreeNode;

	new_child->SetParentNode(this);
	new_child->SetStartTag(start_tag);
	new_child->SetEndTag(end_tag);

	AddChildNode(new_child);

	if(start_tag == end_tag) {
		XML_Content new_content;

		new_content.SetContentTag(start_tag);
		AddNodeContent(new_content);
	}

	else {
		XML_Content new_content;

		new_content.SetContentTag(start_tag);
		AddNodeContent(new_content);

		new_content.SetContentTag(end_tag);
		AddNodeContent(new_content);
	}

	return new_child;
}

XML_TreeNode* XML_TreeNode::CreateChildFromString(string content_string) {
	XML_TreeNode* new_child = new XML_TreeNode;

	XML_Content new_content;
	new_content.SetContentString(content_string, true);

	new_child->SetParentNode(this);
	new_child->AddNodeContent(new_content);

	AddChildNode(new_child);
	AddNodeContent(new_content);

	return new_child;
}

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
			label = "<" + start_tag->GetTagName() + "/>";

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
