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

    vecSorts.clear();

    try
    {
        DatabaseUtils::queryAllSorts(database(), false, vecSorts);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    if (vecSorts.size() <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "No more records";
        response().out() << json();
        return;
    }

    json()["data"] = vecSorts;
    json()["error"] = "null";
    response().out() << json();
}

void SettingsService::sliderImages()
{
    SliderImages vecImages;

    vecImages.clear();

    try
    {
        DatabaseUtils::queryAllSliderImages(database(), vecImages);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    if (vecImages.size() <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "No more records";
        response().out() << json();
        return;
    }

    json()["data"] = vecImages;
    json()["error"] = "null";
    response().out() << json();
}

void SettingsService::websiteOptions()
{
    options vecOptions;
    vecOptions.clear();

    try
    {
        DatabaseUtils::queryAllOptions(database(), vecOptions);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    if (vecOptions.size() <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "No more records";
        response().out() << json();
        return;
    }

    json()["data"] = vecOptions;
    json()["error"] = "null";
    response().out() << json();
}
