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
    static bool insertSliderImage(cppdb::session&, const SliderImage& recoder);
    static bool deleteSliderImage(cppdb::session&, const SliderImage& recoder);
    static bool updateSliderImage(cppdb::session&, const SliderImage& recoder);

    //菜单栏
    static void querySortByArticleId(cppdb::session& sql, int nArticleId, sort& record);
    static void queryAllSorts(cppdb::session&, sorts& vecRes);
    static bool insertSort(cppdb::session&, const sort& recoder);
    static bool deleteSort(cppdb::session&, const sort& recoder);
    static bool updateSort(cppdb::session&, const sort& recoder);

    //网站设置
    static void queryAllOptions(cppdb::session&, options& vecRes);
    static bool insertOption(cppdb::session&, const option& recoder);
    static bool deleteOption(cppdb::session&, const option& recoder);
    static bool updateOption(cppdb::session&, const option& recoder);

    //文章操作
    static void queryArticles(cppdb::session&, std::string strCondition, int nStart, int nShowCount, articles& vecRes);
    static void queryArticleById(cppdb::session&, int nId, article& resArticle);
    static bool insertArticle(cppdb::session&, const article& recoder);
    static bool deleteArticle(cppdb::session&, const article& recoder);
    static bool updateArticle(cppdb::session&, const article& recoder);

    //用户操作
    static void queryUsers(cppdb::session&, articles& vecRes);
    static void queryUserById(cppdb::session&, int nId, user& resUser);
    static bool insertUser(cppdb::session&, const user& recoder);
    static bool deleteUser(cppdb::session&, const user& recoder);
    static bool updateUser(cppdb::session&, const user& recoder);

    //评论操作
    static void queryCommentsByArticleId(cppdb::session&, int nArticleId, comments& vecRes);
    static void queryCommentsByUserId(cppdb::session&, int nUserId, comments& vecRes);
    static bool insertComment(cppdb::session&, comment& recoder);
    static bool deleteComment(cppdb::session&, comment& recoder);
    static bool updateComment(cppdb::session&, comment& recoder);
};

#endif //DATABASE_UTLIS_H