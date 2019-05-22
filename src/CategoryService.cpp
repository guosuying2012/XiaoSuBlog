#include "CategoryService.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>
#include <cppcms/json.h>

CategoryService::CategoryService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &CategoryService::index, this);
    dispatcher().map("GET", "/getSortArticles/(\\d+)(/(\\d+))", &CategoryService::articleSortList, this, 1, 3);
    dispatcher().map("GET", "/getAuthorArticles/(\\d+)(/(\\d+))", &CategoryService::articleAuthorList, this, 1, 3);
    mapper().root("/xiaosu");
}

CategoryService::~CategoryService()
{
}

void CategoryService::index()
{
    //API LIST
    response().out() << "category";
}

void CategoryService::articleSortList(int nSortId, int nCount)
{
    cppcms::json::value jsonRes;
    articles vecRes;
    int nRowCount;
    std::string strCondition;
    std::stringstream ss;

    vecRes.clear();
    nRowCount = 10;
    strCondition.clear();
    ss.clear();

    ss << "yengsu_set_article_sort.sort_id IN ( SELECT sort_id FROM yengsu_sorts WHERE sort_parent_id = " 
    << nSortId << " OR sort_id = " << nSortId << ")";
    strCondition = ss.str();

    try
    {
        DatabaseUtils::queryArticles(database(), strCondition, nCount, nRowCount, vecRes);
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

void CategoryService::articleAuthorList(int nAuthorId, int nCount)
{
    cppcms::json::value jsonRes;
    articles vecRes;
    int nRowCount;
    std::string strCondition;
    std::stringstream ss;

    vecRes.clear();
    nRowCount = 10;
    strCondition.clear();
    ss.clear();

    ss << "yengsu_users.user_id = " << nAuthorId;
    strCondition = ss.str();

    try
    {
        DatabaseUtils::queryArticles(database(), strCondition, nCount, nRowCount, vecRes);
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
