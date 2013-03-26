#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
using namespace std;

#include "xml_tag.h"

#ifndef XML_DOCUMENT
#define XML_DOCUMENT

struct XML_Content {
	XML_Content() {
		content_string = "";
		content_tag = NULL;
	}

	int print_content() {
		if(content_tag == NULL)
			printf("content: \"%s\"\n\n", content_string.c_str());

		else {
			content_tag->PrintTagInfo();
			printf("\n");
		}

		return true;
	}

	string content_string;
	XML_Tag* content_tag;
};

class XML_Document {
public:
	XML_Document();
	~XML_Document();

	////////////////////////////////
	// File i/o related functions //
	////////////////////////////////

	int LoadFromFile(FILE* handle);
	int LoadFromFile(const char* filename);

	int WriteToFile(FILE* handle);
	int WriteToFile(const char* filename);

	//////////////////////////////
	// Content access functions //
	//////////////////////////////

	unsigned int GetContentCount();
	XML_Content GetContent(unsigned int index);

private:
	//The data describing this document
	vector<XML_Content> document_content;
};

#endif
