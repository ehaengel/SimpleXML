#include <stdlib.h>
#include <stdio.h>

#include "xml_tag.h"
#include "xml_document.h"

int main(int argc, char** argv) {
	/*XML_Tag* tag = new XML_Tag;
	if(tag->LoadFromFile(argv[1]) == false) {
		printf("Error loading tag\n");
		return 1;
	}

	tag->PrintTagInfo();*/

	XML_Document doc;
	doc.LoadFromFile(argv[1]);

	for(unsigned int i=0; i<doc.GetContentCount(); i++) {
		doc.GetContent(i).print_content();
	}

	return 0;
}
