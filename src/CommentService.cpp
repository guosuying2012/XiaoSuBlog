#include "CommentService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

#include <sstream>
#include <exception>

CommentService::CommentService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &CommentService::index, this);
    dispatcher().map("GET", "/getCommentByArticleId/(\\d+)", &CommentService::commentByArticleId, this, 1);
    dispatcher().map("GET", "/getCommentById/(\\d+)", &CommentService::commentById, this, 1);
    dispatcher().map("POST", "/postComment", &CommentService::postComment, this);
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

void CommentService::commentById(int nCommentId)
{
    comment record;
    record.clear();

    try
    {
        DatabaseUtils::queryCommentById(database(), nCommentId, record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response().out() << json();
        return;
    }

    json()["data"] = record;
    json()["error"] = "null";
    response().out() << json();
}

void CommentService::postComment()
{
    bool bFlag;
    int nInseredId;
    cppcms::json::value object;
    comment record;

    bFlag = false;
    nInseredId = 0;
    record.clear();

    try
    {
        std::istringstream ss(request().post("comment"));
        object.load(ss, true);
        record = object.get_value<comment>();
        bFlag = DatabaseUtils::insertComment(database(), nInseredId, record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        json().null();
        json()["data"] = "null";
        json()["error"] = e.what();
        response().out() << json();
        return;
    }
    catch (std::exception const& e)
    {
        json().null();
        json()["data"] = "null";
        json()["error"] = e.what();
        response().out() << json();
        return;
    }
    catch (...)
    {
        json().null();
        json()["data"] = "null";
        json()["error"] = "Unknow exception";
        response().out() << json();
        return;
    }

    if (bFlag)
    {
        json().null();
        json()["data"] = "评论成功!";
        json()["error"] = "null";
        json()["inseredid"] = nInseredId;
        response().out() << json();
    }
    else
    {
        json().null();
        json()["data"] = "null";
        json()["error"] = "评论失败!";
        response().out() << json();
    }
}
