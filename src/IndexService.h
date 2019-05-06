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
    void _default();
    void article_list();
    void navigation_bar();
    void slider_images();
    void test_database(std::string strId);
};

#endif  //INDEX_SERVICE_H