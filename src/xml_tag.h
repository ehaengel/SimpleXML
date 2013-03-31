#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <string>
#include <vector>
using namespace std;

#ifndef XML_TAG
#define XML_TAG

struct XML_TagAttribute {
	XML_TagAttribute() {
		attribute_name = "";
		attribute_value = "";
	}

	string attribute_name;
	string attribute_value;
};

class XML_Tag {
public:
	XML_Tag();
	~XML_Tag();

	////////////////////////////////
	// File i/o related functions //
	////////////////////////////////

	int LoadFromFile(FILE* handle, bool skip_open_bracket);
	int LoadFromFile(const char* filename);

	int WriteToFile(FILE* handle);
	int WriteToFile(const char* filename);

	///////////////////////////
	// Tag parsing functions //
	///////////////////////////

	int ParseTagString(string tag_string);

	///////////////////////////////
	// Tag information functions //
	///////////////////////////////

	int SetTagString(string tag_string);
	string GetTagString();

	int SetTagName(string tag_name);
	string GetTagName();

	int SetTagType(int tag_type);
	int GetTagType();

	int AppendTagAttribute(XML_TagAttribute tag_attribute);
	int AppendTagAttribute(string attribute_name, string attribute_value);
	int AppendTagAttribute(string attribute_name, double attribute_value);
	int AppendTagAttribute(string attribute_name, int attribute_value);

	int RemoveTagAttritube(unsigned int index);

	unsigned int GetAttributeCount();
	XML_TagAttribute GetAttribute(unsigned int index);

	string GetAttributeName(unsigned int index);
	string GetAttributeValue(unsigned int index);
	string GetAttributeValue(string attribute_name);

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int PrintTagInfo();


	// Enumeration of tag types
	enum {
		UNDEF_TAG,
		OPEN_TAG,
		CLOSE_TAG,
		OPENCLOSE_TAG
	};

private:
	//Internal use functions
	int update_tag_string();

	//Tag data
	string tag_string;

	string tag_name;
	int tag_type;

	vector<XML_TagAttribute> tag_attributes;
};

#endif
