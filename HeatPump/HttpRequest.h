#pragma once
#include "Arduino.h"

typedef enum {
	GET = 0,
	POST = 1
} RequestType;

class HttpRequest
{
public:

	RequestType type;
	String URL;
	String host;
	String body;
	//String** Parameters;
	int NumberParms=0;
	String* ParamKeys;
	String* ParamValues;
	void ParseParameters();
	int getIndexOfParmKey(String key);
	String getParmValue(String key);
	String getParmValue(int indexKey);

	HttpRequest();
	~HttpRequest();
private:


};

