#include <stdlib.h>
#include <stdio.h>

#include "xml_tag.h"
#include "xml_document.h"

int main(int argc, char** argv) {
	XML_Document* xml_document = new XML_Document;

	xml_document->LoadFromFile(argv[1]);

	vector<XML_TreeNode*> result;
	xml_document->GetHeadNode()->GetTreeNodesOfTagName(string(argv[2]), result);

	for(unsigned int i=0; i<result.size(); i++) {
		XML_Tag* start_tag = result[i]->GetStartTag();
		if(start_tag != NULL) {
			printf("%s %u: ", start_tag->GetTagName().c_str(), i);

			printf("%u\n", result[i]->CountTagsOfTagName(string(argv[3])));
		}
	}

	return 0;
}
