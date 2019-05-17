#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 
#include <cppdb/frontend.h>

IndexService::IndexService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().assign("", &IndexService::index, this);
    mapper().assign("");

    dispatcher().assign("/getArticles/(\\d+)", &IndexService::articleList, this, 1);
    mapper().assign("getArticles", "/getArticles/{1}");

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

