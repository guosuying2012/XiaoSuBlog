#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 
#include <cppdb/frontend.h>

#include <iostream>

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
    cppcms::json::value jsonRes;
    jsonRes["data"] = "null";

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
    catch(std::exception const& e)
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
    catch(std::exception const& e)
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


