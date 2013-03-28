#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
using namespace std;

#include "xml_tag.h"
#include "xml_content.h"
#include "xml_tree.h"

#ifndef XML_DOCUMENT
#define XML_DOCUMENT

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

	/////////////////////////////////
	// XML document tree functions //
	/////////////////////////////////

	int CreateDocumentTree();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int WriteDotFile(const char* filename);

private:
	//The data describing this document
	vector<XML_Content> document_content;

	//The data describing the document tree
	XML_TreeNode* head_node;
};

#endif
