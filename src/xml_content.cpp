#include "xml_content.h"

XML_Content::XML_Content() {
	content_string = "";
	content_tag = NULL;
}

XML_Content::~XML_Content() {
	// Do nothing
}

/////////////////////////
// Setters and Getters //
/////////////////////////

int XML_Content::SetContentString(string content_string, bool use_trim_whitespace) {
	if(use_trim_whitespace == true)
		this->content_string = trim_whitespace(content_string);

	else
		this->content_string = content_string;

	return true;
}

string XML_Content::GetContentString() {
	return content_string;
}

int XML_Content::SetContentTag(XML_Tag* content_tag) {
	this->content_tag = content_tag;

	return true;
}

XML_Tag* XML_Content::GetContentTag() {
	return content_tag;
}

/////////////////////////
// Debugging functions //
/////////////////////////

int XML_Content::print_content() {
	if(content_tag == NULL)
		printf("content: \"%s\"\n\n", content_string.c_str());

	else {
		content_tag->PrintTagInfo();
		printf("\n");
	}

	return true;
}

//Internal use functions
string XML_Content::trim_whitespace(string str) {
	string res = "";

	const char* buf = str.c_str();

	char* p1 = (char*) buf;
	while(*p1 && (*p1 == ' ' || *p1 == '\n' || *p1 == '\t'))
		p1++;

	char* p2 = (char*) buf;
	while(*p2)
		p2++;
	p2--;

	while(p2 > p1 && (*p2 == ' ' || *p2 == '\n' || *p2 == '\t'))
		p2--;

	char* p = p1;
	while(p <= p2) {
		res += *p;
		p++;
	}

	return res;
}
