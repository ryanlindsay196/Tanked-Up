#include "QueryLoader.h"
#include <stdlib.h>
using namespace std;

QueryLoader::QueryLoader()
{
	curl = curl_easy_init();
}

QueryLoader::QueryLoader(char* in_queryEndpoint)
{
	QueryLoader();
	endpointLink = in_queryEndpoint;
}

QueryLoader::~QueryLoader()
{
	curl_easy_cleanup(curl);
}



char* QueryLoader::LoadValueChars(const char* in_key)
{
	if (returnedStringFromWebPage == "null")
		return (char*)"";
	return (char*)document[in_key].GetString();
}

bool QueryLoader::LoadValueBool(const char * in_key)
{
	if (returnedStringFromWebPage == "null")
		return false;
	else
		return document[in_key].GetBool();
}

int QueryLoader::LoadValueInt(const char* in_key)
{
	if (returnedStringFromWebPage == "null")
		return 0;
	return document[in_key].GetInt();
}

float QueryLoader::LoadValueFloat(const char* in_key)
{
	if (returnedStringFromWebPage == "null")
		return 0.0f;
	return document[in_key].GetFloat();
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	//printf(mem->memory);

	return realsize;
}

void QueryLoader::LoadPage(char* in_queryLink)
{
	CURLcode res;

	std::cout << "Loading page" << std::endl;
	
	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(1); //will be grown as needed by the realloc above
	chunk.size = 0;//no data at this point

	curl_global_init(CURL_GLOBAL_ALL);

	//init the curl session
	curl = curl_easy_init();

	//specify the URL to get
	curl_easy_setopt(curl, CURLOPT_URL, in_queryLink);

	//send all data to this function
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ::WriteCallback);

	//we pass our 'chunk' struct to the callback function
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

	//some servers don't like requests that are made without a user-agent field, so we provide one
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	//get it!
	res = curl_easy_perform(curl);

	//check for errors
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
	else 
	{
		//Now, our chunk.memory points to a memory block that us chunk.size bytes big and contains the remote file.
		//Do something nice with it!

		printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
	}

	//cleanup curl stuff
	curl_easy_cleanup(curl);
	printf(chunk.memory);
	returnedStringFromWebPage = chunk.memory;
	//queryData->LoadQueryData(chunk.memory);
	document.Parse(chunk.memory);
	//printf("\n\n");
	//printf("Username is: ");
	//printf(document[0]["UserName"].GetString());
	//printf("\n\n");
	//printf(document[1]["UserName"].GetString());
	//printf("\n\n");
	//printf(document[2]["UserName"].GetString());
	//printf("\n\n");
	//printf(document.RawNumber);

	free(chunk.memory);

	//we're done with libcurl, so clean it up
	curl_global_cleanup();

#pragma region DUD


	//if (curl) 
	//{
	//	curl_easy_setopt(curl, CURLOPT_URL, (std::string)in_endpointLink + (std::string)in_queryLink);
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//	curl_easy_cleanup(curl);
	//
	//	//std::cout << readBuffer << std::endl;
	//}


	//char* returnStuff = (char*)"HELLO";
	//
	//ofstream fp;
	//fp.open("poop.txt");
	//
	//CURLcode res = CURLE_OK;
	//if ((res = curl_easy_setopt(curl, CURLOPT_URL, in_endpointLink)) != CURLE_OK)
	//{
	//	std::cout << "QueryLoader::LoadPage(char*,char*)::Error loading curl link";
	//	return (char*)"QueryLoader::LoadPage(char*,char*)::Error loading curl link";
	//}
	//
	//if ((res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)) != CURLE_OK)
	//{
	//	std::cout << "QueryLoader::LoadPage(char*, char*)::";
	//	return (char*)"QueryLoader::LoadPage(char*, char*)::Error with CURLOPT_FOLLOWLOCATION thing";
	//}
	//
	//return returnStuff;
	//char* returnStuff = strcpy()
	//return (char*)readBuffer.c_str();
#pragma endregion

}

std::string QueryLoader::ReturnedStringFromWebPage()
{
	return returnedStringFromWebPage;
}
