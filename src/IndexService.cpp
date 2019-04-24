#include "IndexService.h"
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

IndexService::IndexService(cppcms::service& srv)
	:BaseService(srv)
{
	dispatcher().assign("/index", &IndexService::_default, this);
	mapper().assign("index", "/index");

	dispatcher().assign("/index/get_article_list", &IndexService::article_list, this);
	mapper().assign("get_article_list", "/index/get_article_list");

	dispatcher().assign("/index/get_navigation_bar", &IndexService::navigation_bar, this);
	mapper().assign("get_navigation_bar", "/index/get_navigation_bar");

	dispatcher().assign("/index/get_slider_images", &IndexService::slider_images, this);
	mapper().assign("get_slider_images", "/index/get_slider_images");

	mapper().root("/xiaosu");
}

IndexService::~IndexService()
{
}

void IndexService::_default()
{
	response().out() << "_default";
}

void IndexService::article_list()
{
	ifstream json_data;
	stringstream ss;
	string strLine;

	json_data.open("./test/article_list.json", ios::in);
	if (!json_data.is_open())
	{
		response().out() << "Open File Failed.";
		return;
	}

    while (getline(json_data, strLine))
    {
		ss << strLine;
    }
    json_data.close();

	response().out() << ss.str();
}

void IndexService::navigation_bar()
{
	ifstream json_data;
	stringstream ss;
	string strLine;

	json_data.open("./test/navigation_bar.json", ios::in);
	if (!json_data.is_open())
	{
		response().out() << "Open File Failed.";
		return;
	}

    while (getline(json_data, strLine))
    {
		ss << strLine;
    }
    json_data.close();

	response().out() << ss.str();
}

void IndexService::slider_images()
{
	ifstream json_data;
	stringstream ss;
	string strLine;

	json_data.open("./test/slider_images.json", ios::in);
	if (!json_data.is_open())
	{
		response().out() << "Open File Failed.";
		return;
	}

    while (getline(json_data, strLine))
    {
		ss << strLine;
    }
    json_data.close();

	response().out() << ss.str();
}

