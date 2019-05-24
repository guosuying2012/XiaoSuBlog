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
    dispatcher().map("GET", "/getArticleById/(\\d+)", &ArticleService::articleById, this, 1);
    mapper().root("/xiaosu");
}

ArticleService::~ArticleService()
{
}

void ArticleService::index()
{
    response().out() << "ArticleService";
}

void ArticleService::articleById(int nArticleId)
{
    article recoder;
    recoder.clear();

    if (nArticleId <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "未找到相关的文章!";
        response(500).out() << json();
        return;
    }

    try
    {
        DatabaseUtils::queryArticleById(database(), nArticleId, recoder);
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
