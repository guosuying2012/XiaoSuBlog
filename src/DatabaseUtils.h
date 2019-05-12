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
    static void queryArticles(cppdb::session&, int nStart, int nShowCount, articles& vecRes);
};

#endif //DATABASE_UTLIS_H