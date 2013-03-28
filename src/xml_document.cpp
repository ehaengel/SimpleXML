#include "xml_document.h"

XML_Document::XML_Document() {
	document_content.clear();

	head_node = NULL;
}

XML_Document::~XML_Document() {
	for(unsigned int i=0; i<document_content.size(); i++)
		if(document_content[i].GetContentTag() != NULL)
			delete document_content[i].GetContentTag();

	document_content.clear();

	if(head_node != NULL)
		delete head_node;
}

////////////////////////////////
// File i/o related functions //
////////////////////////////////

int XML_Document::LoadFromFile(FILE* handle) {
	char p = 0;

	string content_string = "";
	while(!feof(handle)) {
		p = fgetc(handle);

		//This is the start of a tag
		if(p == '<') {
			//First clear out the content string
			if(content_string != "") {
				XML_Content new_content;
				new_content.SetContentString(content_string, true);
				//new_content.content_string = content_string;
				
				if(new_content.GetContentString() != "")
					document_content.push_back(new_content);

				content_string = "";
			}

			//Next try to load in a new tag
			XML_Tag* tag = new XML_Tag;

			if(tag->LoadFromFile(handle, true) == false) {
				delete tag;
				return false;
			}

			XML_Content new_content;
			new_content.SetContentTag(tag);

			document_content.push_back(new_content);
		}
		else if(!feof(handle))
			content_string += p;
	}

	//Save any remaining content
	if(content_string != "") {
		XML_Content new_content;
		new_content.SetContentString(content_string, true);
		//new_content.content_string = content_string;

		if(new_content.GetContentString() != "")
			document_content.push_back(new_content);
	}

	return true;
}

int XML_Document::LoadFromFile(const char* filename) {
	FILE* handle = fopen(filename, "r");
	if(!handle)
		return false;

	int ret = LoadFromFile(handle);

	fclose(handle);
	return ret;
}

int XML_Document::WriteToFile(FILE* handle) {
	return true;
}

int XML_Document::WriteToFile(const char* filename) {
	FILE* handle = fopen(filename, "w");
	if(!handle)
		return false;

	int ret = LoadFromFile(handle);

	fclose(handle);
	return ret;
}

//////////////////////////////
// Content access functions //
//////////////////////////////

unsigned int XML_Document::GetContentCount() {
	return document_content.size();
}

XML_Content XML_Document::GetContent(unsigned int index) {
	return document_content[index];
}

/////////////////////////////////
// XML document tree functions //
/////////////////////////////////

int XML_Document::CreateDocumentTree() {
	head_node = new XML_TreeNode;

	head_node->SetNodeContent(document_content);
	if(head_node->ParseContent() == false) {
		delete head_node;
		head_node = NULL;
		return false;
	}

	//printf("Number of children nodes: %u\n", head_node->GetChildrenNodeCount());

	return true;
}

/////////////////////////
// Debugging functions //
/////////////////////////

int XML_Document::WriteDotFile(const char* filename) {
	if(head_node == NULL)
		return false;

	if(head_node->WriteDotFile(filename) == false)
		return false;

	return true;
}
