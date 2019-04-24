#include <iostream>
#include "HttpClient.h"

bool http_client::s_bInit = false;

http_client::http_client() 
{
	if (!s_bInit) 
	{
		curl_global_init(CURL_GLOBAL_ALL);
		s_bInit = true;
	}
	m_timeout = 0;
	m_bDebug = false;
}

http_client::~http_client() 
{
	curl_global_cleanup();
}

int http_client::send(http_request& req, http_response& res)
{
	return curl(req, res);
}

static size_t s_write_cb(char *buf, size_t size, size_t cnt, void *userdata) 
{
	if (buf == NULL || userdata == NULL) return 0;

	http_response* pRes = (http_response*)userdata;
	pRes->append(buf, size*cnt);
	return size * cnt;
}

int http_client::curl(http_request& req, http_response& res) 
{
	if (m_bDebug)
	{
		std::cout << req.method.c_str() << req.url.c_str() << req.text.c_str() << std::endl;
	}

	CURL* handle = curl_easy_init();

	//options

	//method
	curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, req.method.c_str());

	//url
	curl_easy_setopt(handle, CURLOPT_URL, req.url.c_str());
	
	//ssl
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);//1L
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);//2L
	//curl_easy_setopt(handle, CURLOPT_CAINFO, "cacert.pem");

	//30x
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

	// header
	struct curl_slist *headers = NULL;
	if (m_headers.size() != 0)
		for (int i = 0; i < m_headers.size(); ++i)
			headers = curl_slist_append(headers, m_headers[i].c_str());

	const char* psz = "text/plain";
	switch (req.content_type) 
	{
#define CASE_CONTENT_TYPE(id, str) \
    case http_request::id: psz = str;    break;
		FOREACH_CONTENT_TYPE(CASE_CONTENT_TYPE)
#undef  CASE_CONTENT_TYPE
	}
	std::string strContentType("Content-type: ");
	strContentType += psz;
	headers = curl_slist_append(headers, strContentType.c_str());
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

	// body or params
	switch (req.content_type) 
	{
	case http_request::NONE:
		break;
	case http_request::FORM_DATA: 
	{
		struct curl_httppost* httppost = NULL;
		struct curl_httppost* lastpost = NULL;
		auto iter = req.form.begin();
		while (iter != req.form.end()) 
		{
			CURLformoption opt = CURLFORM_COPYCONTENTS;
			if (iter->second.type == FormData::FILENAME) 
			{
				opt = CURLFORM_FILE;
			}
			curl_formadd(&httppost, &lastpost,
				CURLFORM_COPYNAME, iter->first.c_str(),
				opt, iter->second.data.c_str(),
				CURLFORM_END);
			iter++;
		}
		if (httppost) 
		{
			curl_easy_setopt(handle, CURLOPT_HTTPPOST, httppost);
		}
	}
		break;
	case http_request::QUERY_STRING:
	case http_request::X_WWW_FORM_URLENCODED:
	{
		std::string params;
		auto iter = req.kvs.begin();
		while (iter != req.kvs.end()) 
		{
			if (iter != req.kvs.begin()) 
			{
				params += '&';
			}
			params += iter->first;
			params += '=';
			params += iter->second;

			iter++;
		}
		if (req.content_type == http_request::QUERY_STRING) 
		{
			std::string url_with_params(req.url);
			url_with_params += '?';
			url_with_params += params;
			curl_easy_setopt(handle, CURLOPT_URL, url_with_params.c_str());
		}
		else 
		{
			curl_easy_setopt(handle, CURLOPT_POSTFIELDS, params.c_str());
		}
	}
		break;
	default: 
		{
			curl_easy_setopt(handle, CURLOPT_POSTFIELDS, req.text.c_str());
		}
		break;
	}

	if (m_timeout != 0) 
	{
		curl_easy_setopt(handle, CURLOPT_TIMEOUT, m_timeout);
	}

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, s_write_cb);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &res);

	int ret = curl_easy_perform(handle);
	if (ret != 0) 
	{
		std::cout << curl_easy_strerror((CURLcode)ret) << std::endl;
	}

	if (headers) 
	{
		curl_slist_free_all(headers);
	}

	curl_easy_cleanup(handle);

	return ret;
}
