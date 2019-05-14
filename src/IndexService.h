#ifndef INDEX_SERVICE_H
#define INDEX_SERVICE_H

#include "BaseService.h"
#include <string>

class IndexService : public BaseService
{
public:
    IndexService(cppcms::service& srv);
    ~IndexService();

private:
    void index();
    void articleList(std::string strCount);
    void navigationBar();
    void sliderImages();
    void websiteOptions();
};

#endif  //INDEX_SERVICE_H