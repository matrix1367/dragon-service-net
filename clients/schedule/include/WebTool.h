/*
 * WebTool.h
 *
 *  Created on: 13 lut 2015
 *      Author: m.bettin
 */

#ifndef WEBTOOL_H_
#define WEBTOOL_H_

#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>

class CWebTool {
public:
	CWebTool();
	virtual ~CWebTool();
	static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp);
	CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30);
	std::string getPage(const char * url);
	std::string getFile(const std::string urlFile);
};

#endif /* WEBTOOL_H_ */
