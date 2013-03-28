#include "xml_tag.h"

XML_Tag::XML_Tag() {
	tag_string = "";

	tag_name = "";
	tag_type = XML_Tag::UNDEF_TAG;

	tag_attributes.clear();
}

XML_Tag::~XML_Tag() {
}

////////////////////////////////
// File i/o related functions //
////////////////////////////////

int XML_Tag::LoadFromFile(FILE* handle, bool skip_open_bracket) {
	if(skip_open_bracket == false) {
		char startc = fgetc(handle);
		if(startc != '<')
			return false;
	}

	string buf = "<";

	int done = false;
	bool inside_quotes = false;

	char p = fgetc(handle);
	while(!feof(handle) && p && !done) {
		buf += p;
		p = fgetc(handle);

		if(p == '\"')
			inside_quotes = !inside_quotes;

		if(inside_quotes == false && p == '>')
			done = true;
	}
	if(p != '>')
		return false;

	buf += p;

	//printf("tag: %s\n", buf.c_str());
	return ParseTagString(buf);
}

int XML_Tag::LoadFromFile(const char* filename) {
	FILE* handle = fopen(filename, "r");
	if(!handle)
		return false;

	int ret = LoadFromFile(handle, false);

	fclose(handle);
	return ret;
}

///////////////////////////
// Tag parsing functions //
///////////////////////////

int XML_Tag::ParseTagString(string tag_string) {
	//printf("%s\n", tag_string.c_str());
	//First check if this is a closed tag
	const char* buffer = tag_string.c_str();

	//Safety check
	if(*buffer != '<')
		return false;

	//This is a close tag
	if(*(buffer+1) == '/')
		tag_type = XML_Tag::CLOSE_TAG;

	//Load the tag name
	char* p = NULL;
	if(tag_type == XML_Tag::CLOSE_TAG)
		p = (char*) (buffer + 2);

	else
		p = (char*) (buffer + 1);

	if(isalpha(*p) == false)
		return false;

	tag_name = "";
	while(*p && *p != ' ' && *p != '\n' && *p !='\t' && *p != '/' && *p != '>') {
		tag_name += *p;
		p++;
	}

	//If this is a closed tag, the tag should be over now
	if(tag_type == XML_Tag::CLOSE_TAG) {
		//Clear out any whitespace
		while(*p && (*p == ' ' || *p == '\n' || *p == '\t'))
			p++;

		if(*p != '>')
			return false;

		this->tag_string = tag_string;
		return true;
	}

	//Otherwise load in any tag attributes
	else {
		int found_attribute = true;
		while(found_attribute == true) {
			found_attribute = false;

			//Clear out any white space
			while(*p && (*p == ' ' || *p == '\n' || *p == '\t'))
				p++;

			if(isalpha(*p) != 0) {
				string attribute_name = "";

				while(*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '=' && *p != '/' && *p != '>') {
					attribute_name += *p;
					p++;
				}

				//Clear out any white space
				while(*p && (*p == ' ' || *p == '\n' || *p == '\t'))
					p++;

				//Jump past the equal sign
				if(*p != '=')
					return false;

				p++;

				//Clear out any white space
				while(*p && (*p == ' ' || *p == '\n' || *p == '\t'))
					p++;

				string attribute_value = "";
				if(*p == '\"') {
					p++;
					while(*p && *p != '\"') {
						attribute_value += *p;
						p++;
					}

					if(*p != '\"')
						return false;

					p++;
				}
				else {
					while(*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '/' && *p != '>') {
						attribute_value += *p;
						p++;
					}
				}

				XML_TagAttribute temp;
				temp.attribute_name = attribute_name;
				temp.attribute_value = attribute_value;

				found_attribute = true;
				tag_attributes.push_back(temp);
			}
		}
	}

	//Clear out any whitespace
	while(*p && (*p == ' ' || *p == '\n' || *p == '\t'))
		p++;

	//Figure out what kind of tag this is
	if(*p == '/' && *(p+1) == '>')
		tag_type = XML_Tag::OPENCLOSE_TAG;

	else if(*p == '>')
		tag_type = XML_Tag::OPEN_TAG;

	else
		return false;

	this->tag_string = tag_string;
	return true;
}

///////////////////////////////
// Tag information functions //
///////////////////////////////

string XML_Tag::GetTagString() {
	return tag_string;
}

string XML_Tag::GetTagName() {
	return tag_name;
}

int XML_Tag::GetTagType() {
	return tag_type;
}

unsigned int XML_Tag::GetAttributeCount() {
	return tag_attributes.size();
}

XML_TagAttribute XML_Tag::GetAttribute(unsigned int index) {
	return tag_attributes[index];
}


/////////////////////////
// Debugging functions //
/////////////////////////

int XML_Tag::PrintTagInfo() {
	printf("Tag string: %s\n", tag_string.c_str());
	printf("Tag name: %s\n", tag_name.c_str());

	printf("Tag type: ");
	switch(tag_type) {
		case XML_Tag::OPEN_TAG:
			printf("open tag\n");
			break;

		case XML_Tag::CLOSE_TAG:
			printf("close tag\n");
			break;

		case XML_Tag::OPENCLOSE_TAG:
			printf("open-close tag\n");
			break;

		default:
			printf("undef\n");
			break;
	}

	printf("Number of tag attributes: %u\n", tag_attributes.size());

	for(unsigned int i=0; i<tag_attributes.size(); i++) {
		const char* name = tag_attributes[i].attribute_name.c_str();
		const char* value = tag_attributes[i].attribute_value.c_str();

		printf("%s = \"%s\"\n", name, value);
	}

	return 0;
}
