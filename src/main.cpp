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
	if(doc.LoadFromFile(argv[1]) == false)
		printf("Error loading the xml document\n");

	for(unsigned int i=0; i<doc.GetContentCount(); i++)
		doc.GetContent(i).print_content();

	if(doc.CreateDocumentTree() == false)
		printf("Error creating the document tree\n");

	if(doc.WriteDotFile(argv[2]) == false)
		printf("Unable to write the dot file\n");

	return 0;
}
