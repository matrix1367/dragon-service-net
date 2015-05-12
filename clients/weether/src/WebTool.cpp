/*
 * WebTool.cpp
 *
 *  Created on: 13 lut 2015
 *      Author: m.bettin
 */

#include "WebTool.h"

CWebTool::CWebTool() {
	// TODO Auto-generated constructor stub

}

CWebTool::~CWebTool() {
	// TODO Auto-generated destructor stub
}


size_t CWebTool::data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if(os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}


CURLcode CWebTool::curl_read(const std::string& url, std::ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if(curl)
	{
		if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}

std::string CWebTool::getPage(const char * url)
{
	curl_global_init(CURL_GLOBAL_ALL);
	std::ostringstream oss;
	if(CURLE_OK == curl_read(url, oss))
	{

		curl_global_cleanup();
		return  oss.str();
	}

	curl_global_cleanup();
	printf("Error getPage, url: %s\n",url );
	return "";
}

std::string convertHttpToFileName(std::string urlFile)
{
	printf("url file path: %s\n", urlFile.c_str());
	std::string path = "";
	std::string::size_type indexSt =0;
	indexSt = urlFile.find_last_of("/");
	if (indexSt != std::string::npos) {
		path = urlFile.substr(indexSt+1, urlFile.length() - indexSt);
	}
	printf("Convert path: %s\n", path.c_str());
	return path;

}

std::string CWebTool::getFile(const std::string urlFile)
{
	std::string pathFile = "";
	/*pathFile = convertHttpToFileName(urlFile);

	curl_global_init(CURL_GLOBAL_ALL);
	std::ofstream ofs(pathFile);
	if(CURLE_OK == curl_read(urlFile.c_str(), ofs))
	{
		curl_global_cleanup();
	}
*/
	return pathFile;
}
