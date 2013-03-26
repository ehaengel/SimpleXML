#include <stdlib.h>
#include <stdio.h>

#include "xml_tag.h"

int main(int argc, char** argv) {
	XML_Tag* tag = new XML_Tag;
	if(tag->LoadFromFile(argv[1]) == false) {
		printf("Error loading tag\n");
		return 1;
	}

	tag->PrintTagInfo();

	return 0;
}
