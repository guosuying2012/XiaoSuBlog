#include "DatabaseUtils.h"
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
    SliderImage recoder;
    recoder.clear();
    resRecords.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT id, image_path, image_description FROM yengsu_slider_images WHERE image_isshow != 0";
        while(resRecords.next())
        {
            resRecords >> recoder.nId >> recoder.strPath >> recoder.strDescription;
            vecRes.push_back(recoder);
        }
    }
    catch(std::exception const& e)
    {
        throw e;
    }
}

//插入记录
bool DatabaseUtils::insertSliderImage(cppdb::session& sql, const SliderImage& recoder)
{
    cppdb::statement stat;
    stat.clear();

    try
    {
        stat = sql << "INSERT INTO yengsu_slider_images(image_path, image_description, image_isshow) VALUES('?', '?', ?)";

        stat.bind(recoder.strPath);
        stat.bind(recoder.strDescription);
        stat.bind(recoder.nIsShow);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//删除记录
bool DatabaseUtils::deleteSliderImage(cppdb::session& sql, const SliderImage& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId < 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_slider_images WHERE id = ?";
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//更新记录
bool DatabaseUtils::updateSliderImage(cppdb::session& sql, const SliderImage& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_slider_images SET image_path='?', image_description='?', image_isshow=? WHERE id = ?";
        stat.bind(recoder.strPath);
        stat.bind(recoder.strDescription);
        stat.bind(recoder.nIsShow);
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true; 
}

//菜单操作
//查询所有分类
void DatabaseUtils::queryAllSorts(cppdb::session& sql, sorts& vecRes)
{
    cppdb::result resRecords;
    sort recoder;
    resRecords.clear();
    recoder.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT sort_id, sort_parent_id, sort_name, sort_rank FROM yengsu_sorts";
        while (resRecords.next())
        {
            resRecords >> recoder.nId >> recoder.nParentId >> recoder.strName >> recoder.nRank;
            vecRes.push_back(recoder);
        }
    }
    catch(std::exception const& e)
    {
        throw e;
    }
}

//增加
bool DatabaseUtils::insertSort(cppdb::session& sql, const sort& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "INSERT INTO yengsu_sorts(sort_parent_id, sort_name, sort_rank) VALUES('?', '?', ?)";
        stat.bind(recoder.nParentId);
        stat.bind(recoder.strName);
        stat.bind(recoder.nRank);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//删除
bool DatabaseUtils::deleteSort(cppdb::session& sql, const sort& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_sorts WHERE sort_id = ?";
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//更新
bool DatabaseUtils::updateSort(cppdb::session& sql, const sort& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_sorts SET sort_parent_id='?', sort_name='?', sort_rank=? WHERE sort_id = ?";
        stat.bind(recoder.nParentId);
        stat.bind(recoder.strName);
        stat.bind(recoder.nRank);
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true; 
}

//网站设置
void DatabaseUtils::queryAllOptions(cppdb::session& sql, options& vecRes)
{
    cppdb::result resRecords;
    option recoder;
    resRecords.clear();
    recoder.clear();
    vecRes.clear();

    try
    {
        resRecords = sql << "SELECT option_id, option_name, option_value FROM yengsu_options";
        while (resRecords.next())
        {
            resRecords >> recoder.nId >> recoder.strName >> recoder.strValue;
            vecRes.push_back(recoder);
        }
    }
    catch(std::exception const& e)
    {
        throw e;
    }
}

bool DatabaseUtils::insertOption(cppdb::session& sql, const option& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "INSERT INTO yengsu_options(option_name, option_value) VALUES('?', '?')";
        stat.bind(recoder.strName);
        stat.bind(recoder.strValue);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

bool DatabaseUtils::deleteOption(cppdb::session& sql, const option& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_sorts WHERE sort_id = ?";
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

bool DatabaseUtils::updateOption(cppdb::session& sql, const option& recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId <= 0)
    {
        return false;
    }

    try
    {
        stat = sql << "UPDATE yengsu_sorts SET option_name='?', option_value='?' WHERE option_id = ?";
        stat.bind(recoder.strName);
        stat.bind(recoder.strValue);
        stat.bind(recoder.nId);
        stat.exec();
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//文章操作
void DatabaseUtils::queryArticles(cppdb::session& sql, int nStart, int nShowCount, articles& vecRes)
{
    cppdb::result resRecords;
    article recoder;
    int nStartNum;

    resRecords.clear();
    recoder.clear();
    vecRes.clear();
    nStartNum = 0;

    nStartNum = nStart == 1 ? 0 : nStart * 10;

    try
    {
        resRecords = sql << "SELECT article_id, user_id, article_title, article_views, article_comment_count \
        article_date, article_like_count, article_describe \
        FROM yengsu_options limit ?,?" 
        << nStartNum << nShowCount;
        while (resRecords.next())
        {
            recoder.nId = resRecords.get<unsigned int>("article_id");
            recoder.strTitle = resRecords.get<std::string>("article_title");
            recoder.nViews = resRecords.get<unsigned int>("article_views");
            recoder.nCommentCount = resRecords.get<unsigned int>("article_comment_count");
            recoder.nTime = resRecords.get<long long>("article_date");
            recoder.nLikeCount = resRecords.get<unsigned int>("article_like_count");
            recoder.strDescribe = resRecords.get<std::string>("article_describe");
            int nUserId = resRecords.get<unsigned int>("user_id");
            cppdb::result res = sql << "SELECT user_id, user_name, user_nikename FROM yengsu_users WHERE user_id=?" << nUserId << cppdb::row;
            if(!res.empty()) 
            {
                res >> recoder.m_user.nId >> recoder.m_user.strName >> recoder.m_user.strNikeName;
                vecRes.push_back(recoder);
            }
        }
    }
    catch(std::exception const& e)
    {
        throw e;
    }
}
