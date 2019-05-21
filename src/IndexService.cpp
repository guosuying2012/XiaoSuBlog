#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

IndexService::IndexService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("", &IndexService::index, this);

    dispatcher().map("GET", "/getArticles/(\\d+)", &IndexService::articleList, this, 1);

    mapper().root("/xiaosu");
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
    cppcms::json::value jsonRes;
    articles vecRes;
    int nRowCount;

    vecRes.clear();
    nRowCount = 10;

    try
    {
        DatabaseUtils::queryArticles(database(), "", nCount, nRowCount, vecRes);
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

