#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

IndexService::IndexService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &IndexService::index, this);
    dispatcher().map("GET", "/getArticles/(\\d+)", &IndexService::articleList, this, 1);
    mapper().root("/index");
}

IndexService::~IndexService()
{
}

void IndexService::index()
{
    //API LIST
    response().out() << "index";
}

void IndexService::articleList(int nCount)
{
    articles vecRes;
    int nRowCount;

    vecRes.clear();
    nRowCount = 10;

    try
    {
        DatabaseUtils::queryArticles(database(), "article_approval_status != 0", nCount, nRowCount, vecRes);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    if (vecRes.size() <= 0)
    {
        json()["data"] = "null";
        json()["error"] = u8"没有找到更多记录!";
        response().out() << json();
        return;
    }

    json()["data"] = vecRes;
    json()["error"] = "null";
    response().out() << json();
}

