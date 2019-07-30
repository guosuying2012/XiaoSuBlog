#include "AdminService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/http_file.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>
#include <cppcms/util.h>
#include <StringTemplate/StringTemplate.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace StringTemplate;

AdminService::AdminService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &AdminService::index, this);
    dispatcher().map("GET", "/publish", &AdminService::admin_publish, this);
    dispatcher().map("GET", "/article", &AdminService::admin_articles, this);
    dispatcher().map("GET", "/users", &AdminService::admin_users, this);
    dispatcher().map("GET", "/system", &AdminService::admin_system, this);
    dispatcher().map("GET", "/article_edit/(\\d+)", &AdminService::article_edit, this, 1);
    dispatcher().map("GET", "/article_delete/(\\d+)", &AdminService::article_delete, this, 1);
    dispatcher().map("GET", "/article_verify/(\\d+)", &AdminService::article_verify, this, 1);
    dispatcher().map("POST", "/postArticle", &AdminService::postArticle, this);
    dispatcher().map("POST", "/uploadImages", &AdminService::uploadImages, this);
    mapper().root("/admin");
}

AdminService::~AdminService()
{

}

void AdminService::index()
{
    Template tpl("./admin/index.html");
    tpl.set("title", "XiaoSu");
    tpl.set("function", "控制台");
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_publish()
{
    sorts vecRes;
    Template tpl("./admin/publish.html");

    vecRes.clear();
    tpl.set("title", "XiaoSu");
    tpl.set("function", "发表文章");

    DatabaseUtils::queryAllSorts(database(), vecRes);
    auto sorts_block = tpl.block("option").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        sorts_block.set("value", vecRes.at(i).strName);
        sorts_block.set("value_id", vecRes.at(i).nId);
        sorts_block = sorts_block.next();
    }
    tpl.set("article_preview", "http://via.placeholder.com/100");

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_articles()
{
    Template tpl("./admin/article.html");
    articles vecRes;
    
    vecRes.clear();
    tpl.set("title", "XiaoSu");
    tpl.set("function", "文章管理");

    DatabaseUtils::queryArticles(database(), "", 1, 20, vecRes);
    auto article_list = tpl.block("article_list").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        std::time_t t(vecRes.at(i).nTime / 1000);
        std::time_t lt(vecRes.at(i).nLastModified / 1000);
        article_list.set("article_id", vecRes.at(i).nId);
        article_list.set("article_title", vecRes.at(i).strTitle);
        article_list.set("article_author", vecRes.at(i).m_user.strDisplayName);
        //article_list.set("article_views", vecRes.at(i).nViews);
        //article_list.set("article_comment", vecRes.at(i).nCommentCount);
        article_list.set("article_sort", vecRes.at(i).m_sort.strName);
        article_list.set("article_data", std::put_time(std::localtime(&t), "[%F %T]"));
        article_list.set("article_last_data", std::put_time(std::localtime(&lt), "[%F %T]"));
        if (vecRes.at(i).bIsApproval)
        {
            article_list.set("article_status", "smile");
            article_list.set("color", "#2083fe");
        }
        else
        {
            article_list.set("article_status", "frown");
            article_list.set("color", "red");
        }
        article_list = article_list.next();
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_users()
{
    Template tpl("./admin/users.html");
    users vecRes;
    
    vecRes.clear();
    tpl.set("title", "XiaoSu");
    tpl.set("function", "用户管理");

    try
    {
        DatabaseUtils::queryUsers(database(), vecRes);
        auto user_list = tpl.block("user_list").repeat(vecRes.size());
        for (int i = 0; i < vecRes.size(); ++i)
        {
            std::time_t t(vecRes.at(i).nRegistrationTime / 1000);
            user_list.set("user_id", vecRes.at(i).nId);
            user_list.set("user_ip", vecRes.at(i).strIp);
            user_list.set("user_nikename", vecRes.at(i).strDisplayName);
            user_list.set("user_username", vecRes.at(i).strName);
            user_list.set("user_regtime", std::put_time(std::localtime(&t), "[%F %T]"));
            user_list.set("user_signature", vecRes.at(i).strSignature);
            user_list = user_list.next();
        }
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("danger", "发生错误", std::string("发生内部错误: ") + e.what());
        return;
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_system()
{
    Template tpl("./admin/system.html");
    tpl.set("function", "系统设置");

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article_edit(int nId)
{
    sorts vecRes;
    article record;
    Template tpl("./admin/publish.html");

    vecRes.clear();
    record.clear();
    tpl.set("title", "XiaoSu");
    tpl.set("function", "文章编辑");

    try
    {
        DatabaseUtils::queryAllSorts(database(), vecRes);
        DatabaseUtils::queryArticleById(database(), nId, record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("danger", "发生错误", std::string("发生内部错误: ") + e.what());
        return;
    }

    auto sorts_block = tpl.block("option").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        sorts_block.set("value", vecRes.at(i).strName);
        sorts_block.set("value_id", vecRes.at(i).nId);
        if (vecRes.at(i).nId == record.m_sort.nId)
        {
            sorts_block.set("selected", "selected");
        }
        sorts_block = sorts_block.next();
    }
    tpl.set("article_title", record.strTitle);
    tpl.set("article_describe", record.strDescribe);
    tpl.set("article_ontent", record.strContent);
    tpl.set("article_id", record.nId);
    if (record.strImage.empty())
    {
        tpl.set("article_preview", "http://via.placeholder.com/100");
    }
    else
    {
        tpl.set("article_preview", record.strImage);
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article_delete(int nId)
{
    article record;
    record.clear();

    record.nId = nId;

    try
    {
        DatabaseUtils::deleteArticle(database(), record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "发生错误", std::string("发生内部错误: ") + e.what());
        return;
    }

    message("success", "删除成功", "博文删除成功");
    return;
}

void AdminService::message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText)
{
    Template tpl("./admin/message.html");
    tpl.set("title", "XiaoSu");
    tpl.set("function", strMsgTitle);
    tpl.set("message_title", strMsgTitle);
    tpl.set("message_type", strMsgType);
    tpl.set("message_content", strMsgText);
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article_verify(int nId)
{
    article record;
    record.clear();

    try
    {
        DatabaseUtils::queryArticleById(database(), nId, record);
        record.bIsApproval = !record.bIsApproval;
        DatabaseUtils::updateArticle(database(), record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "状态修改失败", std::string("状态修改失败: ") + e.what());
        return;
    }

    if (record.bIsApproval)
    {
        message("success", "博文审核成功", "博文审核成功，现在转到前台即可查看博文！");
    }
    else
    {
        message("success", "状态已更改", "博文状态已被更改，现在转到文章管理可预览博文！");
    }
    return;
}

void AdminService::postArticle()
{
    std::string strTitle;
    std::string strSortId;
    std::string strDescribe;
    std::string strContent;
    cppcms::http::request::files_type vecFiles;
    std::ostringstream ostrFileName;
    std::string strPostToken;
    std::ostringstream ostrFilePath;
    std::string strId;
    struct article record;

    strTitle.clear();
    strSortId.clear();
    strDescribe.clear();
    strContent.clear();
    vecFiles.clear();
    ostrFileName.clear();
    strPostToken.clear();
    ostrFilePath.clear();
    strId.clear();
    record.clear();

    strTitle = request().post("title");
    strSortId = request().post("sort");
    strDescribe = request().post("describe");
    strContent = request().post("content");
    strPostToken = request().post("posttoken");
    strId = request().post("article_id");
    if (strPostToken == m_strPostToken)
    {
        message("danger", "发布错误", "请勿刷新页面！");
        return;
    }
    else
    {
        m_strPostToken = strPostToken;
    }

    vecFiles = request().files();
    for (std::size_t i = 0; i < vecFiles.size(); ++i)
    {
        const std::string& strFileName = vecFiles.at(i)->filename();
        if (strFileName.empty())
        {
            continue;
        }

        const std::string& strSuffix = strFileName.substr(strFileName.find_last_of('.'));
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        ostrFileName << setting("htdocs") << "/assets/upload/cover/" << ms.count() << strSuffix;
        vecFiles.at(i)->save_to(ostrFileName.str());
        ostrFilePath << setting("host") << "/assets/upload/cover/" << ms.count() << strSuffix;
        ostrFileName.clear();
    }

    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );

    if (!strId.empty())
    {
        try
        {
            DatabaseUtils::queryArticleById(database(), std::stoi(strId, nullptr, 0), record);
            record.strTitle = strTitle;
            record.m_sort.nId = std::stoi(strSortId,nullptr,0);
            record.strDescribe = cppcms::util::escape(strDescribe);
            record.strContent = strContent;
            record.nLastModified = ms.count();
            if (ostrFilePath.rdbuf()->in_avail() != 0)
            {
                record.strImage = ostrFilePath.str();
            }
            DatabaseUtils::updateArticle(database(), record);
        }
        catch (cppdb::cppdb_error const& e)
        {
            message("warning", "更新失败", std::string("博文更新失败: ") + e.what());
            return;
        }

        message("success", "更新成功", "博客已成功刷新！");
        return;
    }

    record.m_user.nId = 1;
    record.m_sort.nId = std::stoi(strSortId, nullptr, 0);
    record.strTitle = strTitle;
    record.strImage = ostrFilePath.str();
    record.strContent = cppcms::util::escape(strContent);
    record.nTime = ms.count();
    record.nLastModified = ms.count();
    record.strDescribe = strDescribe;

    try
    {
        DatabaseUtils::insertArticle(database(), record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "发布失败", e.what());
    }

    message("success", "发布成功", "您的博客发表成功！");
}

void AdminService::uploadImages()
{
    std::string strFileName;
    cppcms::http::request::files_type vecFiles;
    std::ostringstream ostrFileName;
    cppcms::json::value jObject; 

    strFileName.clear();
    vecFiles.clear();
    ostrFileName.clear();

    vecFiles = request().files();
    for (std::size_t i = 0; i < vecFiles.size(); ++i)
    {
        const std::string& strFileName = vecFiles.at(i)->filename();
        const std::string& strSuffix = strFileName.substr(strFileName.find_last_of('.'));
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        ostrFileName << setting("htdocs") << "/assets/upload/images/" << ms.count() << strSuffix;
        vecFiles.at(i)->save_to(ostrFileName.str());

        std::ostringstream ostrPath;
        ostrPath << setting("host") << "/assets/upload/images/" << ms.count() << strSuffix;
        jObject["success"] = 1;
        jObject["message"] = "上传成功!";
        jObject["url"] = ostrPath.str();
        response().out() << jObject;

        ostrFileName.clear();
    }
}

