#include "DatabaseUtils.h"
#include <iostream>

DatabaseUtils::DatabaseUtils()
{
    
}

DatabaseUtils::~DatabaseUtils()
{
    
}

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
            vecRes.push_back(resRecords);
        }
    }
    catch(std::exception const& e)
    {
        throw e;
    }
}

//插入记录
bool DatabaseUtils::insertSliderImages(cppdb::session& sql, SliderImages& vecRes)
{
    cppdb::statement stat;
    size_t nLength;
    stat.clear();
    nLength = 0;

    try
    {
        stat = sql << "INSERT INTO yengsu_slider_images(image_path, image_description, image_isshow) VALUES('?', '?', ?)";
        nLength = vecRes.size();
        for (size_t i = 0; i < nLength; ++i)
        {
            const SliderImage& image = vecRes.at(i);
            stat.bind(image.strPath);
            stat.bind(image.strDescription);
            stat.bind(image.nIsShow);
            stat.exec();
        } 
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//删除记录
bool DatabaseUtils::deleteSliderImage(cppdb::session& sql, SliderImage recoder)
{
    cppdb::statement stat;
    stat.clear();

    if (recoder.nId < 0)
    {
        return false;
    }

    try
    {
        stat = sql << "DELETE FROM yengsu_slider_images WHERE id = ?" << recoder.nId;
    }
    catch(std::exception const& e)
    {
        throw e;
    }

    return stat.affected() <= 0 ? false : true;
}

//更新记录
bool DatabaseUtils::updateSliderImages(cppdb::session& sql, SliderImage recoder)
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

