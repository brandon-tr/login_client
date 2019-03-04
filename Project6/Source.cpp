// ConsoleApplication4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <stdio.h>
#include <string>
#include "json.hpp"
#include <sstream>

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment(lib, "curl/libcurl_a_debug.lib")

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size * nmemb;
	try
	{
		s->append((char*)contents, newLength);
	}
	catch (std::bad_alloc &e)
	{
		return 0;
	}
	return newLength;
}

int main()
{
	CURL *curl;
	CURLcode res;
	using json = nlohmann::json;
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();



	std::string username;
	std::cin >> username;
	std::string password;
	std::cin >> password;
	std::string data = "username=" + username + "&password=" + password;
	std::string s;
	
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/postTest");
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);


		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	auto jsonParse = json::parse(s);
	if (jsonParse["success"].empty() != true)
	{
		std::cout << "asdasdad" << std::endl;
		std::cout << jsonParse["success"] << std::endl;
	}
	else if (jsonParse["failure"].empty() != true)
	{
		std::cout << jsonParse["failure"] << std::endl;
	}
	else
	{
		std::cout << "Invalid response sent or received" << std::endl;
	}
	std::cout << jsonParse << std::endl;
}
