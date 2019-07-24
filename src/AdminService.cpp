#include "AdminService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/http_file.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>
#include <cppcms/util.h>

#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>

AdminService::AdminService(cppcms::service& srv)
    :BaseService(srv)
{
    m_nIndex = -1;
    dispatcher().map("GET", "", &AdminService::index, this);
    dispatcher().map("GET", "/publish", &AdminService::publish, this);
    dispatcher().map("GET", "/article", &AdminService::article, this);
    dispatcher().map("GET", "/users", &AdminService::users, this);
    dispatcher().map("GET", "/system", &AdminService::system, this);
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
    tpl.set("function", "控制台");
    m_nIndex = 0;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::publish()
{
    Template tpl("./admin/publish.html");
    sorts vecRes;

    vecRes.clear();
    tpl.set("function", "发表文章");
    m_nIndex = 1;
    renderMenu(tpl);

    DatabaseUtils::queryAllSorts(database(), vecRes);
    auto sorts_block = tpl.block("option").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        sorts_block.set("value", vecRes.at(i).strName);
        sorts_block.set("value_id", vecRes.at(i).nId);
        sorts_block = sorts_block.next();
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article()
{
    articles vecRes;
    Template tpl("./admin/article.html");
    
    vecRes.clear();
    tpl.set("function", "文章管理");
    m_nIndex = 2;
    renderMenu(tpl);

    DatabaseUtils::queryArticles(database(), "", 1, 20, vecRes);
    auto article_list = tpl.block("article_list").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        std::time_t t(vecRes.at(i).nTime / 1000);
        std::time_t lt(vecRes.at(i).nLastModified / 1000);
        article_list.set("article_id", vecRes.at(i).nId);
        article_list.set("article_title", vecRes.at(i).strTitle);
        article_list.set("article_author", vecRes.at(i).m_user.strDisplayName);
        article_list.set("article_views", vecRes.at(i).nViews);
        article_list.set("article_comment", vecRes.at(i).nCommentCount);
        article_list.set("article_like", vecRes.at(i).nLikeCount);
        article_list.set("article_data", std::put_time(std::localtime(&t), "[%F %T]"));
        article_list.set("article_last_data", std::put_time(std::localtime(&lt), "[%F %T]"));
        article_list = article_list.next();
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::users()
{
    Template tpl("./admin/users.html");
    tpl.set("function", "用户管理");
    m_nIndex = 3;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::system()
{
    Template tpl("./admin/system.html");
    tpl.set("function", "系统设置");
    m_nIndex = 4;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::edit()
{

}

void AdminService::message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText)
{
    Template tpl("./admin/message.html");
    tpl.set("function", strMsgTitle);
    tpl.set("message_title", strMsgTitle);
    tpl.set("message_type", strMsgType);
    tpl.set("message_content", strMsgText);
    m_nIndex = 1;
    renderMenu(tpl);
    tpl.render(response(200, "text/html").out(), true);
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
    struct article record;

    strTitle.clear();
    strSortId.clear();
    strDescribe.clear();
    strContent.clear();
    vecFiles.clear();
    ostrFileName.clear();
    strPostToken.clear();
    ostrFilePath.clear();
    record.clear();

    strTitle = request().post("title");
    strSortId = request().post("sort");
    strDescribe = request().post("describe");
    strContent = request().post("content");
    strPostToken = request().post("posttoken");
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

    record.m_user.nId = 1;
    record.m_sort.nId = std::stoi(strSortId,nullptr,0);
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

void AdminService::renderMenu(Template& tpl)
{
    std::vector<std::string> vecMenuTitles{ "控制台", "发表文章", "文章管理", "用户管理", "系统设置" };
    std::vector<std::string> vecMenuLinks{ "/xiaosu/admin", "/xiaosu/admin/publish", "/xiaosu/admin/article", "/xiaosu/admin/users", "/xiaosu/admin/system" };
    std::vector<std::string> vecMenuIcons{ "home", "pen-tool", "inbox", "users", "settings" };

    if (vecMenuTitles.size() != vecMenuLinks.size())
    {
        return;
    }

    tpl.set("title", "XiaoSu");
    
    auto group = tpl.block("menu_group").repeat(vecMenuTitles.size());
    for (int i = 0; i < vecMenuTitles.size(); ++i)
    {
        group.set("menu_text", vecMenuTitles.at(i));
        group.set("menu_link", vecMenuLinks.at(i));
        group.set("menu_icon", vecMenuIcons.at(i));

        if (i == m_nIndex)
        {
            group.set("menu_active", "is-active");
        }

        group = group.next();
    }
}

