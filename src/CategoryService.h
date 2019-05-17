#ifndef CATEGOTY_SERVICE_H
#define CATEGOTY_SERVICE_H

#include "BaseService.h"

class CategoryService : public BaseService
{
public:
    CategoryService(cppcms::service& srv);
    ~CategoryService();
    
private:
    virtual void index() override;
};

#endif  //CATEGOTY_SERVICE_H