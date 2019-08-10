#include "AdminService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/http_file.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>
#include <cppcms/util.h>
#include <cppcms/session_interface.h>
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
    dispatcher().map("GET", "/article_edit/(.*)", &AdminService::article_edit, this, 1);
    dispatcher().map("GET", "/article_delete/(.*)", &AdminService::article_delete, this, 1);
    dispatcher().map("GET", "/article_verify/(.*)", &AdminService::article_verify, this, 1);
    dispatcher().map("GET", "/user_edit/(\\d+)", &AdminService::user_edit, this, 1);
    dispatcher().map("GET", "/user_disable/(\\d+)", &AdminService::user_disable, this, 1);
    dispatcher().map("POST", "/post_user", &AdminService::postUser, this);
    dispatcher().map("POST", "/postArticle", &AdminService::postArticle, this);
    dispatcher().map("POST", "/uploadImages", &AdminService::uploadImages, this);
    dispatcher().map("POST", "/signin", &AdminService::signin, this);
    dispatcher().map("GET", "/signin", &AdminService::signin, this);
    dispatcher().map("GET", "/signout", &AdminService::signout, this);
    mapper().root("/admin");
}

AdminService::~AdminService()
{

}

void AdminService::index()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    Template tpl("./admin/index.html");
    tpl.set("title", "XiaoSu");
    tpl.set("function", "控制台");
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::signin()
{
    if (request().request_method () == "GET")
    {
        Template tpl("./admin/signin.html");
        tpl.set("title", "XiaoSu");
        tpl.render(response(200, "text/html").out(), true);
        return;
    }

    unsigned long nId;
    std::string user_name;
    std::string user_password;
    bool bFlag = false;

    nId = 0;
    user_name.clear();
    user_password.clear();
    
    user_name = request().post("user_name");
    user_password = request().post("user_password");

    if (user_name.empty() || user_password.empty())
    {
        json()["data"] = "null";
        json()["error"] = u8"账户不能输入空";
        response().out() << json();
        return;
    }

    try
    {
        //加密密码
        bFlag = DatabaseUtils::signin(database(), user_name, user_password, nId);
    }
    catch(cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = u8"登陆失败，系统内部发生错误！";
        response().out() << json();
        return;
    }

    if (bFlag)
    {
        session().set("user_id", nId);
        json()["data"] = "success";
        json()["error"] = "null";
        response().out() << json();
        return;
    }

    json()["data"] = "null";
    json()["error"] = u8"登陆失败！检查账户";
    response().out() << json();
}

void AdminService::signout()
{
    session().clear();
    index();
}

void AdminService::admin_publish()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

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
    tpl.set("model_title", "需要自动生成简介吗？");
    tpl.set("model_conent", "您的简介不满足条件，系统将为您自动剪裁到符合条件的字符长度！");

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_articles()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    Template tpl("./admin/article.html");
    articles vecRes;
    unsigned int nId;
    std::stringstream ssCondition;

    vecRes.clear();
    tpl.set("title", "XiaoSu");
    tpl.set("function", "文章管理");
    nId = 0;
    ssCondition.clear();

    nId = session().get<unsigned long>("user_id");
    if (nId != 1)
    {
        ssCondition << "yengsu_users.user_id="<<nId;
    }

    DatabaseUtils::queryArticles(database(), ssCondition.str(), 1, 20, vecRes);
    auto article_list = tpl.block("article_list").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        std::time_t t(vecRes.at(i).nTime / 1000);
        std::time_t lt(vecRes.at(i).nLastModified / 1000);
        article_list.set("article_id", vecRes.at(i).strId);
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
        if (vecRes.at(i).strImage.empty())
        {
            article_list.set("article_image", "http://via.placeholder.com/72");
        }
        else
        {
            article_list.set("article_image", vecRes.at(i).strImage);
        }
        article_list = article_list.next();
    }

    tpl.set("model_title", "确定要删除这条博文吗？");
    tpl.set("model_conent", "此操作将不可撤销，确定要删除此博文吗？");
    tpl.set("model_link", "article_delete");
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_users()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    Template tpl("./admin/users.html");
    users vecRes;
    std::stringstream ssCondition;
    unsigned long nId;
    
    vecRes.clear();
    ssCondition.clear();
    nId = 0;

    tpl.set("title", "XiaoSu");
    tpl.set("function", "用户管理");
    nId = session().get<unsigned long>("user_id");

    try
    {
        if (nId != 1)
        {
            ssCondition << "user_id=" << nId;
        }
        DatabaseUtils::queryUsers(database(), ssCondition.str(), vecRes);
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
            if (vecRes.at(i).strProfilePhoto.empty())
            {
                user_list.set("user_profile", "http://via.placeholder.com/72");
            }
            else
            {
                user_list.set("user_profile", vecRes.at(i).strProfilePhoto);
            }
            
            if (vecRes.at(i).bIsDisable)
            {
                user_list.set("user_status", "frown");
                user_list.set("color", "red");
            }
            else
            {
                user_list.set("user_status", "smile");
                user_list.set("color", "#2083fe");
            }
            user_list = user_list.next();
        }
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("danger", "发生错误", std::string("发生内部错误: ") + e.what());
        return;
    }

    tpl.set("model_title", "确定要更改此账户的禁用状态？");
    tpl.set("model_conent", "禁用状态后此账户将不能正常使用博客功能，解除禁用将恢复账户的使用。");
    tpl.set("model_link", "user_disable");
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::admin_system()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    Template tpl("./admin/system.html");
    tpl.set("function", "系统设置");

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article_edit(std::string strId)
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

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
        DatabaseUtils::queryArticleById(database(), strId, record);
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
    tpl.set("article_id", record.strId);
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

void AdminService::article_delete(std::string strId)
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    article record;
    record.clear();

    record.strId = strId;

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

void AdminService::article_verify(std::string strId)
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    article record;
    record.clear();

    try
    {
        DatabaseUtils::queryArticleById(database(), strId, record);
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

void AdminService::user_edit(int nId)
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    user record;
    Template tpl("./admin/userinfo_edit.html");
    tpl.set("title", "XiaoSu");
    tpl.set("function", "用户信息编辑");
    record.clear();

    try
    {
        DatabaseUtils::queryUserById(database(), nId, record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "发生内部错误", e.what());
        return;
    }

    std::time_t t(record.nRegistrationTime / 1000);
    if (record.strProfilePhoto.empty())
    {
        tpl.set("user_preview", "http://via.placeholder.com/100");
    }
    else
    {
        tpl.set("user_preview", record.strProfilePhoto);
    }
    
    tpl.set("user_id", record.nId);
    tpl.set("user_name", record.strName);
    tpl.set("user_email", record.strEmail);
    tpl.set("user_nikename", record.strNikeName);
    tpl.set("user_ip", record.strIp);
    tpl.set("user_time", std::put_time(std::localtime(&t), "[%F %T]"));
    tpl.set("user_signature", record.strSignature);
    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::user_disable(int nId)
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    unsigned long nUserId;
    user record;

    nUserId = 0;
    record.clear();

    nUserId = session().get<unsigned long>("user_id");

    if (nUserId != 1)
    {
        message("warning", "状态修改失败", std::string("状态修改失败: 你没有这个权限！"));
        return;
    }

    try
    {
        DatabaseUtils::queryUserById(database(), nId, record);
        record.bIsDisable = !record.bIsDisable;
        DatabaseUtils::updateUser(database(), record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "状态修改失败", std::string("状态修改失败: ") + e.what());
        return;
    }

    if (record.bIsDisable)
    {
        message("success", "该账户已被禁用", "该账户已被禁用！");
    }
    else
    {
        message("success", "该账户已解除禁用", "该账户已解除禁用！");
    }
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

void AdminService::postUser()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    int nId;
    std::string strEmail;
    std::string strNikeName;
    std::string strSignature;
    std::string strProfilePhoto;
    cppcms::http::request::files_type vecFiles;
    std::ostringstream ostrFileName;
    std::ostringstream ostrFilePath;
    struct user record;

    nId = 0;
    strEmail.clear();
    strNikeName.clear();
    strSignature.clear();
    strProfilePhoto.clear();
    vecFiles.clear();
    ostrFileName.clear();
    ostrFilePath.clear();
    record.clear();

    nId = std::stoi(request().post("user_id"));
    strEmail = request().post("email");
    strNikeName = request().post("nikename");
    strSignature = request().post("signature");

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

    if (ostrFilePath.rdbuf()->in_avail())
    {
        strProfilePhoto = ostrFilePath.str();
    }

    try
    {
        DatabaseUtils::queryUserById(database(), nId, record);
        record.strEmail = strEmail;
        record.strNikeName = strNikeName;
        record.strSignature = cppcms::util::escape(strSignature);
        record.strProfilePhoto = strProfilePhoto;
        DatabaseUtils::updateUser(database(), record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        message("warning", "发生内部错误", e.what());
        return;
    }
    message("success", "更新成功", "用户信息已被更新！");
    return;
}

void AdminService::postArticle()
{
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

    std::string strTitle;
    std::string strSortId;
    std::string strDescribe;
    std::string strContent;
    cppcms::http::request::files_type vecFiles;
    std::ostringstream ostrFileName;
    std::ostringstream ostrFilePath;
    std::string strId;
    struct article record;

    strTitle.clear();
    strSortId.clear();
    strDescribe.clear();
    strContent.clear();
    vecFiles.clear();
    ostrFileName.clear();
    ostrFilePath.clear();
    strId.clear();
    record.clear();

    strTitle = request().post("title");
    strSortId = request().post("sort");
    strDescribe = request().post("describe");
    strContent = request().post("content");
    strId = request().post("article_id");

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
            DatabaseUtils::queryArticleById(database(), strId, record);
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

    record.m_user.nId = session().get<unsigned int>("user_id");
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
    if (!session().is_set("user_id"))
    {
        signin();
        return;
    }

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

