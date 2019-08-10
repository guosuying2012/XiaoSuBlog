#ifndef DATABASE_UTLIS_H
#define DATABASE_UTLIS_H

#include "support.h"
#include <cppdb/frontend.h>
#include <vector>
#include <string>
#include <sstream>
#include <random>

class DatabaseUtils
{
public:
    DatabaseUtils();
    ~DatabaseUtils();

    //导航条图片操作
    static void queryAllSliderImages(cppdb::session&, SliderImages& vecRes);
    static bool insertSliderImage(cppdb::session&, const SliderImage& record);
    static bool deleteSliderImage(cppdb::session&, const SliderImage& record);
    static bool updateSliderImage(cppdb::session&, const SliderImage& record);

    //菜单栏
    static void querySortByArticleId(cppdb::session& sql, std::string strId, sort& record);
    static void querySortById(cppdb::session& sql, int nId, sort& record);
    static void queryAllSorts(cppdb::session&, sorts& vecRes);
    static bool insertSort(cppdb::session&, const sort& record);
    static bool deleteSort(cppdb::session&, const sort& record);
    static bool updateSort(cppdb::session&, const sort& record);

    //网站设置
    static void queryAllOptions(cppdb::session&, options& vecRes);
    static bool insertOption(cppdb::session&, const option& record);
    static bool deleteOption(cppdb::session&, const option& record);
    static bool updateOption(cppdb::session&, const option& record);

    //文章操作
    static void queryArticles(cppdb::session&, std::string strCondition, int nStart, int nShowCount, articles& vecRes);
    static void queryArticleById(cppdb::session&, std::string strId, article& resArticle);
    static bool insertArticle(cppdb::session&, const article& record);
    static bool deleteArticle(cppdb::session&, const article& record);
    static bool updateArticle(cppdb::session&, const article& record);

    //用户操作
    static void queryUsers(cppdb::session&, std::string strCondition, users& vecRes);
    static void queryUserById(cppdb::session&, int nId, user& resUser);
    static bool insertUser(cppdb::session&, const user& record);
    static bool deleteUser(cppdb::session&, const user& record);
    static bool updateUser(cppdb::session&, const user& record);

    //评论操作
    static void queryCommentById(cppdb::session&, int nId, comment& record);
    static void queryCommentsByArticleId(cppdb::session&, int nArticleId, comments& vecRes);
    static void queryCommentsByUserId(cppdb::session&, int nUserId, comments& vecRes);
    static bool insertComment(cppdb::session&, int& nInseredId, comment& record);
    static bool deleteComment(cppdb::session&, comment& record);
    static bool updateComment(cppdb::session&, comment& record);

    static bool signin(cppdb::session&, std::string strUserName, std::string strPassword, unsigned long& nId);

    static unsigned int random_char() 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return dis(gen);
    }

    static std::string generate_hex(const unsigned int len) 
    {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) 
        {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << rc;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }
};

#endif //DATABASE_UTLIS_H