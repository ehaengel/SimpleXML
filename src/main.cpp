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

	if(doc.WriteToFile(argv[2]) == false)
		printf("Error writing the xml document\n");

	//for(unsigned int i=0; i<doc.GetContentCount(); i++)
	//	doc.GetContent(i).print_content();

	/*if(doc.CreateDocumentTree() == false) {
		printf("Error creating the document tree\n");
		return 1;
	}*/

	/*if(doc.GetHeadNode() != NULL) {
		vector<XML_TreeNode*> search_results;
		doc.GetHeadNode()->GetTreeNodesOfTagName(string(argv[2]), search_results);

		for(unsigned int i=0; i<search_results.size(); i++) {
			vector<XML_Tag*> search_results2;
			printf("search result %u: %p\n", i, search_results[i]);

			search_results[i]->GetTagsOfTagName(string(argv[3]), search_results2);
			printf("Under tag %u there are %u\n", i, search_results2.size());
		}
	}*/

	/*if(doc.WriteDotFile(argv[3]) == false) {
		printf("Unable to write the dot file\n");
		return 1;
	}*/

	return 0;
}
