#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
using namespace std;

#include "xml_tag.h"

#ifndef XML_CONTENT
#define XML_CONTENT

class XML_Content {
public:
	XML_Content();
	~XML_Content();

	/////////////////////////
	// Setters and Getters //
	/////////////////////////

	int SetContentString(string content_string, bool use_trim_whitespace);
	string GetContentString();

	int SetContentTag(XML_Tag* content_tag);
	XML_Tag* GetContentTag();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int print_content();

private:
	//Internal use function
	string trim_whitespace(string str);

	//Data representing some xml content
	string content_string;
	XML_Tag* content_tag;
};

#endif
