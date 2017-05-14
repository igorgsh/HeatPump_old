#include "HttpRequest.h"
#include "Definitions.h"


HttpRequest::HttpRequest()
{
}


HttpRequest::~HttpRequest()
{
}

void HttpRequest::ParseParameters() {
	
	String line = "";

	if (type == GET) {
		int ind = URL.indexOf("?");
		if (ind > 0) {
			line = URL.substring(ind + 1);
		}
	}
	else if (type == POST) {
		line = body;
	}
	if (line.length() != 0) {
		//count number of parameters
		int numb = 1;
		int ind = 0;
		while ((ind = line.indexOf("&", ind+1)) != -1) {
			numb++;
		}
		NumberParms = numb;
		ParamKeys = new String[numb]; 
		ParamValues = new String[numb];

		ind = 0;
		for (int i = 0; i < numb; i++)
		{
			String parm;
			int ind1;
			ind1 = line.indexOf("&", ind);
			if (ind1 == -1) {
				parm = line.substring(ind);
			}
			else {
				parm = line.substring(ind, ind1);
			}
			ind = ind1 + 1;
			ind1 = parm.indexOf("=");
			ParamKeys[i] = parm.substring(0, ind1);
			ParamValues[i] = parm.substring(ind1 + 1);
		}
	}
}

int HttpRequest::getIndexOfParmKey(String key) {
	for (int i = 0; i < NumberParms; i++)
	{
		if (key.equals(ParamKeys[i])) {
			return i;
		}
	}
	return -1;
}

String HttpRequest::getParmValue(String key) {
	int ind = getIndexOfParmKey(key);
	if (ind != -1) {
		return getParmValue(ind);
	}
	else {
		return "";
	}
}

String HttpRequest::getParmValue(int indexKey) {
	return ParamValues[indexKey];
}
