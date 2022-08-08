#include "pch.h"
#include "Translate.h"
#include <curl/curl.h>
bool GetInterfaceInfo(std::string url, std::string sendjson, std::string* resjson);
size_t WriteMemoryCallback(void* data, size_t size, size_t nmemb, void* userp);
__inline std::string W_To_A(std::wstring wstr);
__inline std::wstring A_To_W(const std::string& str);
struct SMemory {
	char* response;
	size_t size;
	SMemory()
	{
		response = (char*)malloc(1);
		size = 0;
	}
	~SMemory()
	{
		free(response);
		response = NULL;
		size = 0;
	}
};
bool Translate(const WCHAR* inputString, WCHAR* outputString)
{
	std::string url = R"(https://api-test.unispc.com/restcloud/download)";
	std::string resJson;
	std::string sendJson = R"({
    "type":"driver_manage"
})";
	if (GetInterfaceInfo(url, sendJson, &resJson))
	{
		std::wstring resJson_W = A_To_W(resJson);
		wmemcpy(outputString, resJson_W.c_str(), resJson_W.size());
		return true;
	}
	else
	{
		outputString = nullptr;
		return false;
	}
}

void InitDllTranslate()
{
	//初始化libcurl
	CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (res != CURLE_OK)
	{
		MessageBoxA(NULL, "init libcurl faile", "", 0);
	}

}

void ReleaseDllTranslate()
{
	curl_global_cleanup();
}




bool GetInterfaceInfo(std::string url, std::string sendjson, std::string* resjson)
{
	CURL* curl = curl_easy_init();
	CURLcode res;
	SMemory m_chunk;
	if (curl)
	{
		struct curl_slist* header_list = nullptr;
		if (sendjson.length() > 0)
		{
			//设置curl的请求头

			//设置请求为post请求
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			header_list = curl_slist_append(NULL,
				"Content-Type:application/json;charset=UTF-8");

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendjson.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

			//设置请求的URL地址
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			//设置post请求的参数


			//设置ssl验证
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			//将数据写入
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&m_chunk);
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//跳过SSH认证
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_chunk);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		}

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			return false;
		}
		*resjson = m_chunk.response;
		if (resjson->length() == 0)
		{
			return false;
		}
		if (header_list != nullptr)
		{
			curl_slist_free_all(header_list);
		}

		curl_easy_cleanup(curl);
		return true;
	}
	return false;
}

size_t WriteMemoryCallback(void* data, size_t size, size_t nmemb, void* userp)
{
	//size_t written = fwrite(data, size, nmemb, (FILE*)userp);
	size_t realsize = size * nmemb;
	struct SMemory* mem = (struct SMemory*)userp;

	char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0;  /* out of memory! */

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

__inline std::string W_To_A(std::wstring wstr)
{
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	if (len <= 0)return result;
	char* buffer = new char[len + 1];
	if (buffer == NULL)return result;
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

__inline std::wstring A_To_W(const std::string& str)
{
	int nstrlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (nstrlen > 0)
	{
		std::wstring wstr(nstrlen, L'\0');
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)wstr.c_str(), nstrlen);
		return std::move(wstr);
	}
	return (L"");
}
