#include "DatabaseUtils.h"
#include <cppcms/util.h>
#include <iostream>

DatabaseUtils::DatabaseUtils()
{
    
}

DatabaseUtils::~DatabaseUtils()
{
    
}

//导航图片操作
//查询所有需要显示的记录
void DatabaseUtils::queryAllSliderImages(cppdb::session& sql, SliderImages& vecRes)
{
    cppdb::result resRecords;
    SliderImage record;
    record.clear();
    resRecords.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT id, image_path, image_description FROM yengsu_slider_images WHERE image_isshow != 0";
        while(resRecords.next())
        {
            resRecords >> record.nId >> record.strPath >> record.strDescription;
            vecRes.push_back(record);
        }
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

//插入记录
bool DatabaseUtils::insertSliderImage(cppdb::session& sql, const SliderImage& record)
{
    cppdb::statement stat;
    stat.clear();

    try
    {
        stat = sql << "INSERT INTO yengsu_slider_images(image_path, image_description, image_isshow) VALUES(?, ?, ?)";

        stat.bind(record.strPath);
        stat.bind(record.strDescription);
        stat.bind(record.nIsShow);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//删除记录
bool DatabaseUtils::deleteSliderImage(cppdb::session& sql, const SliderImage& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId < 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_slider_images WHERE id = ?";
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//更新记录
bool DatabaseUtils::updateSliderImage(cppdb::session& sql, const SliderImage& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_slider_images SET image_path=?, image_description=?, image_isshow=? WHERE id = ?";
        stat.bind(record.strPath);
        stat.bind(record.strDescription);
        stat.bind(record.nIsShow);
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true; 
}

void DatabaseUtils::querySortByArticleId(cppdb::session& sql, int nArticleId, sort& record)
{
    cppdb::result resRecords;
    resRecords.clear();
    record.clear();

    try
    {
        resRecords = sql << "SELECT sort_id FROM yengsu_set_article_sort WHERE article_id = ?" << nArticleId << cppdb::row;
        if (resRecords.empty())
        {
            throw cppdb::cppdb_error("未找到相关记录");
            return;
        }
        resRecords >> record.nId;
        resRecords = sql << "SELECT sort_parent_id, sort_name, sort_rank FROM yengsu_sorts WHERE sort_id = ?" << record.nId << cppdb::row;
        if (resRecords.empty())
        {
            throw cppdb::cppdb_error("未找到相关记录");
            return;
        }
        resRecords >> record.nParentId >> record.strName >> record.nRank;
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

void DatabaseUtils::querySortById(cppdb::session& sql, int nId, sort& record)
{
    cppdb::result resRecord;
    resRecord.clear();
    record.clear();

    try
    {
        resRecord = sql << "SELECT sort_id, sort_parent_id, sort_name, sort_rank FROM yengsu_sorts WHERE sort_id = ?" << nId << cppdb::row;
        if (resRecord.empty())
        {
            throw cppdb::cppdb_error("未找到相关记录!");
        }
        resRecord >> record.nId >> record.nParentId >> record.strName >> record.nRank;
    }
    catch (cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

//查询所有分类
void DatabaseUtils::queryAllSorts(cppdb::session& sql, sorts& vecRes)
{
    cppdb::result resRecords;
    sort record;
    resRecords.clear();
    record.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT sort_id, sort_parent_id, sort_name, sort_rank FROM yengsu_sorts";
        while (resRecords.next())
        {
            resRecords >> record.nId >> record.nParentId >> record.strName >> record.nRank;
            vecRes.push_back(record);
        }
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

//增加
bool DatabaseUtils::insertSort(cppdb::session& sql, const sort& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "INSERT INTO yengsu_sorts(sort_parent_id, sort_name, sort_rank) VALUES(?, ?, ?)";
        stat.bind(record.nParentId);
        stat.bind(record.strName);
        stat.bind(record.nRank);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//删除
bool DatabaseUtils::deleteSort(cppdb::session& sql, const sort& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_sorts WHERE sort_id = ?";
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//更新
bool DatabaseUtils::updateSort(cppdb::session& sql, const sort& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_sorts SET sort_parent_id=?, sort_name=?, sort_rank=? WHERE sort_id = ?";
        stat.bind(record.nParentId);
        stat.bind(record.strName);
        stat.bind(record.nRank);
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true; 
}

//网站设置
void DatabaseUtils::queryAllOptions(cppdb::session& sql, options& vecRes)
{
    cppdb::result resRecords;
    option record;
    resRecords.clear();
    record.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT option_id, option_name, option_value FROM yengsu_options";
        while (resRecords.next())
        {
            resRecords >> record.nId >> record.strName >> record.strValue;
            vecRes.push_back(record);
        }
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

bool DatabaseUtils::insertOption(cppdb::session& sql, const option& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "INSERT INTO yengsu_options(option_name, option_value) VALUES(?, ?)";
        stat.bind(record.strName);
        stat.bind(record.strValue);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

bool DatabaseUtils::deleteOption(cppdb::session& sql, const option& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_sorts WHERE sort_id = ?";
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

bool DatabaseUtils::updateOption(cppdb::session& sql, const option& record)
{
    cppdb::statement stat;
    stat.clear();

    if (record.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_sorts SET option_name=?, option_value=? WHERE option_id = ?";
        stat.bind(record.strName);
        stat.bind(record.strValue);
        stat.bind(record.nId);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//文章操作
void DatabaseUtils::queryArticles(cppdb::session& sql, std::string strCondition, int nStart, int nShowCount, articles& vecRes)
{
    cppdb::result resRecords;
    article record;
    std::stringstream ssSQL;
    int nStartNum;

    resRecords.clear();
    record.clear();
    vecRes.clear();
    nStartNum = 0;
    ssSQL.clear();

    if (nShowCount <= 0)
    {
        return;
    }

    if (!strCondition.empty())
    {
        strCondition = "AND " + strCondition;
    }

    nStartNum = nStart <= 1 ? 0 : (nStart - 1) * nShowCount;

    try
    {
        ssSQL << "SELECT \
                    yengsu_articles.article_id, \
                    article_title, \
                    article_image, \
                    article_describe, \
                    article_date, \
                    article_comment_count, \
                    yengsu_articles.user_id, \
                    user_name, \
                    user_nikename, \
                    yengsu_set_article_sort.sort_id, \
                    yengsu_sorts.sort_name \
                FROM \
                    yengsu_articles, \
                    yengsu_users, \
                    yengsu_set_article_sort, \
                    yengsu_sorts \
                WHERE \
                    yengsu_users.user_id = yengsu_articles.user_id \
                    AND yengsu_set_article_sort.sort_id = yengsu_sorts.sort_id \
                    AND yengsu_set_article_sort.article_id = yengsu_articles.article_id "
                << strCondition
                << " ORDER BY article_date DESC"
                << " LIMIT " << nStartNum << ", " << nShowCount;

        resRecords = sql << ssSQL.str();
        while (resRecords.next())
        {
            std::string strDescribe = resRecords.get<std::string>("article_describe");
            record.nId = resRecords.get<unsigned int>("article_id");
            record.strTitle = resRecords.get<std::string>("article_title");
            record.strImage = resRecords.get<std::string>("article_image");
            record.nCommentCount = resRecords.get<unsigned int>("article_comment_count");
            record.nTime = resRecords.get<unsigned long long>("article_date");
            record.strDescribe = cppcms::util::escape(strDescribe);
            record.m_user.nId = resRecords.get<unsigned int>("user_id");
            record.m_user.strName = resRecords.get<std::string>("user_name");
            record.m_sort.nId = resRecords.get<unsigned int>("sort_id");
            record.m_sort.strName = resRecords.get<std::string>("sort_name");
            if (resRecords.is_null("user_nikename"))
            {
                record.m_user.strDisplayName = resRecords.get<std::string>("user_name");
            }
            else
            {
                record.m_user.strDisplayName = resRecords.get<std::string>("user_nikename");
            }
            vecRes.push_back(record);
        }
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

void DatabaseUtils::queryArticleById(cppdb::session& sql, int nId, article& resArticle)
{
    cppdb::result resRecord;
    resRecord.clear();
    resArticle.clear();

    try
    {
        resRecord = sql << "SELECT \
                    article_id, \
                    user_id, \
                    article_title, \
                    article_image, \
                    article_content, \
                    article_views, \
                    article_comment_count, \
                    article_date, \
                    article_like_count, \
                    article_last_modified, \
                    article_describe \
                FROM yengsu_articles \
                WHERE article_id = ?" << nId << cppdb::row;

        if (resRecord.empty())  
        {
            throw cppdb::cppdb_error("未找到相关的文章!");
            return;
        }

        resArticle.nId = resRecord.get<unsigned int>("article_id");
        resArticle.m_user.nId = resRecord.get<unsigned int>("user_id");
        resArticle.strTitle = resRecord.get<std::string>("article_title");
        resArticle.strImage = resRecord.get<std::string>("article_image");
        resArticle.strContent = resRecord.get<std::string>("article_content");
        resArticle.nViews = resRecord.get<unsigned int>("article_views");
        resArticle.nCommentCount = resRecord.get<unsigned int>("article_comment_count");
        resArticle.nTime = resRecord.get<unsigned long long>("article_date");
        resArticle.nLikeCount = resRecord.get<unsigned int>("article_like_count");
        resArticle.nLastModified = resRecord.get<unsigned long long>("article_last_modified");
        resArticle.strDescribe = resRecord.get<std::string>("article_describe");
        DatabaseUtils::querySortByArticleId(sql, resArticle.nId, resArticle.m_sort);
        DatabaseUtils::queryUserById(sql, resArticle.m_user.nId, resArticle.m_user);
    }
    catch (cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

bool DatabaseUtils::insertArticle(cppdb::session& sql, const article& record)
{
    return true;
}

bool DatabaseUtils::deleteArticle(cppdb::session& sql, const article& record)
{
    return true;
}

bool DatabaseUtils::updateArticle(cppdb::session& sql, const article& record)
{
    return true;
}

//用户操作
void DatabaseUtils::queryUsers(cppdb::session& sql, articles& vecRes)
{

}

void DatabaseUtils::queryUserById(cppdb::session& sql, int nId,user& resUser)
{
    cppdb::result resRecord;
    resRecord.clear();

    try
    {
        resRecord = sql << "SELECT \
                    user_id, \
                    user_name, \
                    user_email, \
                    user_level, \
                    user_profile_photo, \
                    user_nikename, \
                    user_signature \
                FROM yengsu_users \
                WHERE user_id = ?" << nId << cppdb::row;
    }
    catch (cppdb::cppdb_error const& e)
    {
        throw e;
    }

    if (resRecord.empty())  
    {
        throw cppdb::cppdb_error("未找到相关用户!");
        return;
    }

    resUser.nId = resRecord.get<unsigned int>("user_id");
    resUser.strName = resRecord.get<std::string>("user_name");
    resUser.strEmail = resRecord.get<std::string>("user_email");
    resUser.strProfilePhoto = resRecord.get<std::string>("user_profile_photo");
    resUser.strLevel = resRecord.get<std::string>("user_level");

    if (resRecord.is_null("user_nikename"))
    {
        resUser.strDisplayName = resRecord.get<std::string>("user_name");
    }
    else
    {
        resUser.strDisplayName = resRecord.get<std::string>("user_nikename");
    }

    if (resRecord.is_null("user_signature"))
    {
        resUser.strSignature = std::string("这个人很懒,什么都没有留下!");
    }
    else
    {
        resUser.strSignature = resRecord.get<std::string>("user_signature");
    }
}

bool DatabaseUtils::insertUser(cppdb::session& sql, const user& record)
{
    return true;
}

bool DatabaseUtils::deleteUser(cppdb::session& sql, const user& record)
{
    return true;
}

bool DatabaseUtils::updateUser(cppdb::session& sql, const user& record)
{
    return true;
}

//评论操作
void DatabaseUtils::queryCommentById(cppdb::session& sql, int nId, comment& record)
{
    cppdb::result resRecord;

    record.clear();
    resRecord.clear();

    try
    {
        resRecord = sql << "SELECT \
                        comment_id, \
                        user_id, \
                        article_id, \
                        comment_like_count, \
                        comment_date, \
                        comment_content, \
                        parent_comment_id \
                    FROM yengsu_comments \
                    WHERE comment_id = ?" << nId << cppdb::row;
        if (resRecord.empty())  
        {
            throw cppdb::cppdb_error("未找到相关记录!");
            return;
        }
        record.nId = resRecord.get<unsigned int>("comment_id");
        record.m_user.nId = resRecord.get<unsigned int>("user_id");
        record.nArticle = resRecord.get<unsigned int>("article_id");
        record.nLikeCount = resRecord.get<unsigned int>("comment_like_count");
        record.nTime = resRecord.get<long long>("comment_date");
        record.strContent = resRecord.get<std::string>("comment_content");
        record.nParentId = resRecord.get<unsigned int>("parent_comment_id");
        DatabaseUtils::queryUserById(sql, record.m_user.nId, record.m_user);
    }
    catch (cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

void DatabaseUtils::queryCommentsByArticleId(cppdb::session& sql, int nArticleId, comments& vecRes)
{
    cppdb::result resRecords;
    comment record;

    record.clear();
    resRecords.clear();

    try
    {
        resRecords = sql << "SELECT \
                        comment_id, \
                        user_id, \
                        comment_like_count, \
                        comment_date, \
                        comment_content, \
                        parent_comment_id \
                    FROM yengsu_comments \
                    WHERE article_id = ?" << nArticleId;
        while (resRecords.next())  
        {
            record.nId = resRecords.get<unsigned int>("comment_id");
            record.m_user.nId = resRecords.get<unsigned int>("user_id");
            record.nLikeCount = resRecords.get<unsigned int>("comment_like_count");
            record.nTime = resRecords.get<long long>("comment_date");
            std::string strContent = resRecords.get<std::string>("comment_content");
            record.strContent = cppcms::util::escape(strContent);
            record.nParentId = resRecords.get<unsigned int>("parent_comment_id");
            DatabaseUtils::queryUserById(sql, record.m_user.nId, record.m_user);
            vecRes.push_back(record);
        }
    }
    catch (cppdb::cppdb_error const& e)
    {
        throw e;
    }
}

void DatabaseUtils::queryCommentsByUserId(cppdb::session& sql, int nUserId, comments& vecRes)
{
    
}

bool DatabaseUtils::insertComment(cppdb::session& sql, int& nInseredId, comment& record)
{
    cppdb::statement stat;
    cppdb::result article_conunt;
    int nArticleCount;

    stat.clear();
    nArticleCount = 0;

    try
    {
        stat = sql << "INSERT INTO yengsu_comments(user_id, article_id, comment_like_count, comment_date, comment_content, parent_comment_id) VALUES(?, ?, ?, ?, ?, ?)";
        stat.bind(record.m_user.nId);
        stat.bind(record.nArticle);
        stat.bind(record.nLikeCount);
        stat.bind(record.nTime);
        stat.bind(record.strContent);
        stat.bind(record.nParentId);
        stat.exec();
        nInseredId = stat.last_insert_id();

        //更新文章评论次数
        article_conunt = sql << "SELECT article_comment_count FROM yengsu_articles WHERE article_id = ?" << record.nArticle << cppdb::row;
        if (!article_conunt.empty())
        {
            nArticleCount = article_conunt.get<unsigned int>("article_comment_count");
        }

        stat.reset();
        stat = sql << "UPDATE yengsu_articles SET article_comment_count=? WHERE article_id = ?";
        stat.bind(nArticleCount+1);
        stat.bind(record.nArticle);
        stat.exec();
    }
    catch(cppdb::cppdb_error const& e)
    {
        throw e;
    }
    return stat.affected() <= 0 ? false : true;
}

bool DatabaseUtils::deleteComment(cppdb::session& sql, comment& record)
{
    return true;
}

bool DatabaseUtils::updateComment(cppdb::session& sql, comment& record)
{
    return true;
}
