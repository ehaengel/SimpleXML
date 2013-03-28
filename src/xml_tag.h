#include <stdlib.h>
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

	///////////////////////////
	// Tag parsing functions //
	///////////////////////////

	int ParseTagString(string tag_string);

	///////////////////////////////
	// Tag information functions //
	///////////////////////////////

	string GetTagString();

	string GetTagName();
	int GetTagType();

	unsigned int GetAttributeCount();
	XML_TagAttribute GetAttribute(unsigned int index);

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
	//Tag data
	string tag_string;

	string tag_name;
	int tag_type;

	vector<XML_TagAttribute> tag_attributes;
};

#endif
