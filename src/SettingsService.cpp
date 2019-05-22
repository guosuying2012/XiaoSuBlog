#include "SettingsService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 
#include <cppdb/frontend.h>

SettingsService::SettingsService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &SettingsService::index, this);
    dispatcher().map("GET", "/getNavigations", &SettingsService::navigationBar, this);
    dispatcher().map("GET", "/getSliderImages", &SettingsService::sliderImages, this);
    dispatcher().map("GET", "/getWebsiteOptions", &SettingsService::websiteOptions, this);
    mapper().root("/xiaosu");
}

SettingsService::~SettingsService()
{
}

void SettingsService::index()
{
    //API LIST
    response().out() << "settings";
}

void SettingsService::navigationBar()
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

void SettingsService::sliderImages()
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

void SettingsService::websiteOptions()
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
