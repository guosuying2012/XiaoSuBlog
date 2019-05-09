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
    static void insertSliderImages(cppdb::session&, SliderImages& vecRes);
    static void deleteSliderImage(cppdb::session&, SliderImage recoder);
    static void updateSliderImages(cppdb::session&, SliderImage recoder);
};

#endif //DATABASE_UTLIS_H