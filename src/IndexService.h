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
    void article_list(std::string strCount);
    void navigation_bar();
    void slider_images();
    void website_options();
};

#endif  //INDEX_SERVICE_H