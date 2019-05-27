#include "CommentService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

CommentService::CommentService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &CommentService::index, this);
    dispatcher().map("GET", "/getCommentByArticleId/(\\d+)", &CommentService::commentByArticleId, this, 1);
    dispatcher().map("POST", "/postReplyComment", &CommentService::replyComment, this);
    mapper().root("/xiaosu");
}

CommentService::~CommentService()
{
}

void CommentService::index()
{
    response().out() << "CommentService";
}

void CommentService::commentByArticleId(int nArticleId)
{
    comments resRecoders;
    resRecoders.clear();

    if (nArticleId <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "未找到相关评论!";
        response().out() << json();
        return;
    }

    try
    {
        DatabaseUtils::queryCommentsByArticleId(database(), nArticleId, resRecoders);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    if (resRecoders.size() <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "未找到相关评论!";
        response().out() << json();
        return;
    }

    json()["data"] = resRecoders;
    json()["error"] = "null";
    response().out() << json();
}

void CommentService::replyComment()
{
    response().out() << request().post("article");
    response().out() << request().post("user");
    response().out() << request().post("reply");
}
