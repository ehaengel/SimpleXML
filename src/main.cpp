#include <stdlib.h>
#include <stdio.h>

#include "xml_tag.h"
#include "xml_document.h"

int main(int argc, char** argv) {
	XML_Document* xml_document = new XML_Document;

	XML_TreeNode* head_node = xml_document->GetHeadNode();

	XML_TreeNode* trianglelist = head_node->CreateChildTagPair("trianglelist");

	for(int i=0; i<10; i++) {
		XML_TreeNode* new_triangle_node = trianglelist->CreateChildTagOpenClosed("triangle");

		XML_Tag* new_triangle_tag = new_triangle_node->GetStartTag();
		new_triangle_tag->AppendTagAttribute("index", i);
	}

	xml_document->WriteToFile(argv[1]);
	xml_document->WriteDotFile(argv[2]);

	return 0;
}
