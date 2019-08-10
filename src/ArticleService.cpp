#include "ArticleService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

ArticleService::ArticleService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &ArticleService::index, this);
    dispatcher().map("GET", "/getArticleById/(.*)", &ArticleService::articleById, this, 1);
    mapper().root("/article");
}

ArticleService::~ArticleService()
{
}

void ArticleService::index()
{
    response().out() << "ArticleService";
}

void ArticleService::articleById(std::string strId)
{
    article recoder;
    recoder.clear();

    try
    {
        DatabaseUtils::queryArticleById(database(), strId, recoder);
    }
    catch (cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    json()["data"] = recoder;
    json()["error"] = "null";
    response().out() << json();
}
