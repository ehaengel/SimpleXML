#include <stdlib.h>
#include <stdio.h>

#include "xml_tag.h"
#include "xml_document.h"

int main(int argc, char** argv) {
	XML_Document* xml_document = new XML_Document;

	if(xml_document->LoadFromFile(argv[1]) == false) {
		printf("Error loading from file\n");
		return 1;
	}

	vector<XML_Tag*> result;
	xml_document->GetHeadNode()->GetOpenCloseTags(result);
	//xml_document->GetHeadNode()->GetTreeNodesOfTagName(string(argv[2]), result);

	for(unsigned int i=0; i<result.size(); i++) {
		XML_Tag* tag = result[i];
		if(tag != NULL)
			printf("%s\n", tag->GetTagName().c_str());
	}

	return 0;
}
