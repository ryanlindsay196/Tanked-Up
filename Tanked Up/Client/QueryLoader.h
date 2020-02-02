#pragma once
#include "curl/curl.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

class QueryLoader
{
public:
	//QueryData* queryData;

	QueryLoader();
	QueryLoader(char* in_endpointLink);
	~QueryLoader();

	char* LoadValueChars(const char* in_key);
	bool LoadValueBool(const char* in_key);
	int LoadValueInt(const char* in_key);
	float LoadValueFloat(const char* in_key);



	CURL* curl;

	//struct MemoryStruct {
	//	char *memory;
	//	size_t size;
	//};

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	///If in_endpointLink is null, use endpointLink
	void LoadPage(char* in_queryLink);

	char* queryLink;
	char* endpointLink;

	std::string ReturnedStringFromWebPage();
private:
	rapidjson::Document document;
	std::string returnedStringFromWebPage;
};
