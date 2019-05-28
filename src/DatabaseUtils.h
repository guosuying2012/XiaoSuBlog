#ifndef DATABASE_UTLIS_H
#define DATABASE_UTLIS_H

#include "support.h"
#include <cppdb/frontend.h>
#include <vector>
#include <string>

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
    static void querySortByArticleId(cppdb::session& sql, int nArticleId, sort& record);
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
    static void queryArticleById(cppdb::session&, int nId, article& resArticle);
    static bool insertArticle(cppdb::session&, const article& record);
    static bool deleteArticle(cppdb::session&, const article& record);
    static bool updateArticle(cppdb::session&, const article& record);

    //用户操作
    static void queryUsers(cppdb::session&, articles& vecRes);
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
};

#endif //DATABASE_UTLIS_H