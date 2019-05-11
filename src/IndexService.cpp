#include "IndexService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h> 
#include <cppdb/frontend.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

IndexService::IndexService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().assign("/index", &IndexService::_default, this);
    mapper().assign("index", "/index");

    dispatcher().assign("/index/get_article_list", &IndexService::article_list, this);
    mapper().assign("get_article_list", "/index/get_article_list");

    dispatcher().assign("/index/get_navigation_bar", &IndexService::navigation_bar, this);
    mapper().assign("get_navigation_bar", "/index/get_navigation_bar");

    dispatcher().assign("/index/get_slider_images", &IndexService::slider_images, this);
    mapper().assign("get_slider_images", "/index/get_slider_images");

    dispatcher().assign("/index/test_database/(\\d+)", &IndexService::test_database, this, 1);
    mapper().assign("test_database", "/index/test_database{1}");

    mapper().root("/xiaosu");
}

IndexService::~IndexService()
{
}

void IndexService::_default()
{
    response().out() << "_default";
}

void IndexService::article_list()
{
    ifstream json_data;
    stringstream ss;
    string strLine;

    json_data.open("./test/article_list.json", ios::in);
    if (!json_data.is_open())
    {
        response().out() << "Open File Failed.";
        return;
    }

    while (getline(json_data, strLine))
    {
        ss << strLine;
    }
    json_data.close();

    response().out() << ss.str();
}

void IndexService::navigation_bar()
{
    ifstream json_data;
    stringstream ss;
    string strLine;

    json_data.open("./test/navigation_bar.json", ios::in);
    if (!json_data.is_open())
    {
        response().out() << "Open File Failed.";
        return;
    }

    while (getline(json_data, strLine))
    {
        ss << strLine;
    }
    json_data.close();

    response().out() << ss.str();
}

void IndexService::slider_images()
{
    SliderImages vecImages;
    size_t nImagesLength;
    cppcms::json::value jsonRes;
    vecImages.clear();
    nImagesLength = 0;

    try
    {
        DatabaseUtils::queryAllSliderImages(database(), vecImages);
    }
    catch(std::exception const& e)
    {
        response().out() << e.what();
    }

    nImagesLength = vecImages.size();
    if (nImagesLength <= 0)
    {
        response().out() << "no images";
    }

    for (int i = 0; i < nImagesLength; ++i)
    {
        cppcms::json::value obj = vecImages.at(i);
        jsonRes["data"][i] = obj;
    }

    response().out() << jsonRes;
}

void IndexService::test_database(std::string strId)
{
    cppcms::json::value json_object;
    user m_user;
    m_user.nId = 3;
    m_user.strIp = "localhost";
    m_user.strName = "yengsu";
    m_user.strEmail = "yengsu@yengsu.com";
    m_user.strProfilePhoto = "http://a.b.com/i.jpg";
    m_user.strLevel = "9999";
    m_user.nRights = 0;
    m_user.nRegistrationTime = 1557402627938;
    m_user.strNikeName = "小苏";

    article m_article;
    m_article.nId = 2;
    m_article.m_user = m_user;
    m_article.strTitle = "My Life !";
    m_article.strContent = "asdasdasdasd";
    m_article.nViews = 999;
    m_article.nCommentCount = 123;
    m_article.nTime = 1557402627938;
    m_article.nLikeCount = 550;
    m_article.nLastModified = 1557402627938;

    comment m_comment;
    m_comment.nId = 1;
    m_comment.m_user = m_user;
    m_comment.m_article = m_article;
    m_comment.nLikeCount = 135;
    m_comment.nTime = 1557402627938;
    m_comment.strContent = "zxcasda";
    m_comment.nParentId = 20;

    json_object = m_comment;
    response().out() << json_object;

    //comment test = json_object.get_value<comment>();

    //response().out() << test.nId << " , " << test.m_user.nId << " , " << test.m_article.nId;

    /*try
    {
        cppcms::json::value json_object;
        cppdb::result res;
        res = database() << "select user_id, user_ip, user_name, user_password, user_email, user_rights, user_nikename from yengsu_users";      
        while (res.next())
        {
            int user_id = 0;
            std::string user_ip;
            std::string user_name;
            std::string user_password;
            std::string user_email;
            int user_rights;
            std::string user_nikename;

            res >> user_id >> user_ip >> user_name >> user_password >> user_email >> user_rights >> user_nikename;
            
            json_object["id"] = user_id;
            json_object["ip"] = user_ip;
            json_object["name"] = user_name;
            json_object["password"] = user_password;
            json_object["email"] = user_email;
            json_object["rights"] = user_rights;
            json_object["nikename"] = user_nikename;

            response().out() << json_object;
        }
    }
    catch(std::exception const &e)
    {
       response().out() << "ERROR: " << e.what();
        return;
    }*/
}
