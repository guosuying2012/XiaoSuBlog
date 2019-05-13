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

    dispatcher().assign("/index/get_article_list/(\\d+)", &IndexService::article_list, this, 1);
    mapper().assign("get_article_list", "/index/get_article_list/{1}");

    dispatcher().assign("/index/get_navigation_bar", &IndexService::navigation_bar, this);
    mapper().assign("get_navigation_bar", "/index/get_navigation_bar");

    dispatcher().assign("/index/get_slider_images", &IndexService::slider_images, this);
    mapper().assign("get_slider_images", "/index/get_slider_images");

    dispatcher().assign("/index/get_website_options", &IndexService::website_options, this);
    mapper().assign("get_website_options", "/index/get_website_options");

    mapper().root("/xiaosu");
}

IndexService::~IndexService()
{
}

void IndexService::index()
{
    response().out() << "index";
}

void IndexService::article_list(std::string strCount)
{
    cppcms::json::value jsonRes;
    articles vecRes;
    std::stringstream ss;
    int nReqCount;
    int nRowCount;
    size_t nArticleLength;

    vecRes.clear();
    ss.clear();
    nRowCount = 10;
    nReqCount = 0;
    nArticleLength = 0;

    ss << strCount;
    ss >> nReqCount;

    try
    {
        DatabaseUtils::queryArticles(database() , nReqCount, nRowCount, vecRes);
    }
    catch(cppdb::cppdb_error const& e)
    {
        response().out() << e.what();
        return;
    }

    nArticleLength = vecRes.size();
    if (nArticleLength <= 0)
    {
        jsonRes["data"] = "null";
    }

    for (int i = 0; i < nArticleLength; ++i)
    {
        cppcms::json::value obj = vecRes.at(i);
        jsonRes["data"][i] = obj;
    }

    response().out() << jsonRes;
}

void IndexService::navigation_bar()
{
    sorts vecSorts;
    size_t nSortLength;
    cppcms::json::value jsonRes;
    vecSorts.clear();
    nSortLength = 0;

    try
    {
        DatabaseUtils::queryAllSorts(database(), vecSorts);
    }
    catch(cppdb::cppdb_error const& e)
    {
        response().out() << e.what();
        return;
    }

    nSortLength = vecSorts.size();
    if (nSortLength <= 0)
    {
        jsonRes["data"] = "null";
    }

    for (int i = 0; i < nSortLength; ++i)
    {
        cppcms::json::value obj = vecSorts.at(i);
        jsonRes["data"][i] = obj;
    }

    response().out() << jsonRes;
}

void IndexService::slider_images()
{
    SliderImages vecImages;
    size_t nImagesLength;
    cppcms::json::value jsonRes;
    vecImages.clear();
    nImagesLength = 0;

    try
    {
        DatabaseUtils::queryAllSliderImages(database(), vecImages);
    }
    catch(cppdb::cppdb_error const& e)
    {
        response().out() << e.what();
        return;
    }

    nImagesLength = vecImages.size();
    if (nImagesLength <= 0)
    {
        jsonRes["data"] = "null";
    }

    for (int i = 0; i < nImagesLength; ++i)
    {
        cppcms::json::value obj = vecImages.at(i);
        jsonRes["data"][i] = obj;
    }

    response().out() << jsonRes;
}

void IndexService::website_options()
{
    options vecOptions;
    size_t nImagesLength;
    cppcms::json::value jsonRes;
    vecOptions.clear();
    nImagesLength = 0;

    try
    {
        DatabaseUtils::queryAllOptions(database(), vecOptions);
    }
    catch(cppdb::cppdb_error const& e)
    {
        response().out() << e.what();
        return;
    }

    nImagesLength = vecOptions.size();
    if (nImagesLength <= 0)
    {
        jsonRes["data"] = "null";
    }

    for (int i = 0; i < nImagesLength; ++i)
    {
        cppcms::json::value obj = vecOptions.at(i);
        jsonRes["data"][i] = obj;
    }

    response().out() << jsonRes;
}
