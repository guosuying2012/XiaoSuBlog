#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 
#include <cppdb/frontend.h>

#include <iostream>
#include <sstream>

IndexService::IndexService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().assign("/index", &IndexService::index, this);
    mapper().assign("index", "/index");

    dispatcher().assign("/index/getArticles/(\\d+)", &IndexService::articleList, this, 1);
    mapper().assign("getArticles", "/index/getArticles/{1}");

    dispatcher().assign("/index/getNavigations", &IndexService::navigationBar, this);
    mapper().assign("getNavigations", "/index/getNavigations");

    dispatcher().assign("/index/getSliderImages", &IndexService::sliderImages, this);
    mapper().assign("getSliderImages", "/index/getSliderImages");

    dispatcher().assign("/index/getWebsiteOptions", &IndexService::websiteOptions, this);
    mapper().assign("getWebsiteOptions", "/index/getWebsiteOptions");

    mapper().root("/xiaosu");
}

IndexService::~IndexService()
{
}

void IndexService::index()
{
    response().out() << "index";
}

void IndexService::articleList(std::string strCount)
{
    cppcms::json::value jsonRes;
    articles vecRes;
    std::stringstream ss;
    int nReqCount;
    int nRowCount;

    vecRes.clear();
    ss.clear();
    nRowCount = 10;
    nReqCount = 0;

    ss << strCount;
    ss >> nReqCount;

    try
    {
        DatabaseUtils::queryArticles(database(), "", nReqCount, nRowCount, vecRes);
    }
    catch(cppdb::cppdb_error const& e)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = e.what();
        response(500).out() << jsonRes;
        return;
    }

    if (vecRes.size() <= 0)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = u8"没有找到更多记录!";
        response().out() << jsonRes;
        return;
    }

    jsonRes["data"] = vecRes;

    response().out() << jsonRes;
}

void IndexService::navigationBar()
{
    sorts vecSorts;
    cppcms::json::value jsonRes;

    vecSorts.clear();

    try
    {
        DatabaseUtils::queryAllSorts(database(), vecSorts);
    }
    catch(cppdb::cppdb_error const& e)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = e.what();
        response(500).out() << jsonRes;
        return;
    }

    if (vecSorts.size() <= 0)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = "No more records";
        response().out() << jsonRes;
        return;
    }

    jsonRes["data"] = vecSorts;
    response().out() << jsonRes;
}

void IndexService::sliderImages()
{
    SliderImages vecImages;
    cppcms::json::value jsonRes;

    vecImages.clear();

    try
    {
        DatabaseUtils::queryAllSliderImages(database(), vecImages);
    }
    catch(cppdb::cppdb_error const& e)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = e.what();
        response(500).out() << jsonRes;
        return;
    }

    if (vecImages.size() <= 0)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = "No more records";
        response().out() << jsonRes;
        return;
    }

    jsonRes["data"] = vecImages;
    response().out() << jsonRes;
}

void IndexService::websiteOptions()
{
    options vecOptions;
    cppcms::json::value jsonRes;

    vecOptions.clear();

    try
    {
        DatabaseUtils::queryAllOptions(database(), vecOptions);
    }
    catch(cppdb::cppdb_error const& e)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = e.what();
        response(500).out() << jsonRes;
        return;
    }

    if (vecOptions.size() <= 0)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = "No more records";
        response().out() << jsonRes;
        return;
    }

    jsonRes["data"] = vecOptions;
    response().out() << jsonRes;
}
