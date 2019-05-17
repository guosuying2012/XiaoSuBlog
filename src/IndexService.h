#ifndef INDEX_SERVICE_H
#define INDEX_SERVICE_H

#include "BaseService.h"

class IndexService : public BaseService
{
public:
    IndexService(cppcms::service& srv);
    ~IndexService();

private:
    virtual void index() override;
    void articleList(std::string strCount);
};

#endif  //INDEX_SERVICE_H