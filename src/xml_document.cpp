#include "xml_document.h"

XML_Document::XML_Document() {
	document_content.clear();
}

XML_Document::~XML_Document() {
	for(unsigned int i=0; i<document_content.size(); i++)
		if(document_content[i].content_tag != NULL)
			delete document_content[i].content_tag;

	document_content.clear();
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
				new_content.content_string = content_string;
				new_content.content_tag = NULL;

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
			new_content.content_string = "";
			new_content.content_tag = tag;

			document_content.push_back(new_content);
		}
		else if(!feof(handle))
			content_string += p;
	}

	//Save any remaining content
	if(content_string != "") {
		XML_Content new_content;
		new_content.content_string = content_string;
		new_content.content_tag = NULL;

		document_content.push_back(new_content);

		content_string = "";
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
