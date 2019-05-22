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
    dispatcher().map("GET", "/getArticle/(\\d+)", &ArticleService::articleById, this, 1);
    mapper().root("/xiaosu");
}

ArticleService::~ArticleService()
{
}

void ArticleService::index()
{
    //API LIST
    response().out() << "article";
}

void ArticleService::articleById(int nArticleId)
{
    article recoder;
    cppcms::json::value jsonRes;
    recoder.clear();

    try
    {
        DatabaseUtils::queryArticleById(database(), nArticleId, recoder);
    }
    catch (cppdb::cppdb_error const& e)
    {
        jsonRes["data"] = "null";
        jsonRes["error"] = e.what();
        response(500).out() << jsonRes;
        return;
    }

    jsonRes["data"] = recoder;
    response().out() << jsonRes;
}
