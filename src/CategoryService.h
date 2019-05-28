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
    void articleSortList(int nSortId, int nCount);
    void articleAuthorList(int nSortId, int nCount);
    void sortById(int nId);
};

#endif  //CATEGOTY_SERVICE_H设置