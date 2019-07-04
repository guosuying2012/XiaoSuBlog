#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include "BaseService.h"

class AdminService : public BaseService
{
public:
    AdminService(cppcms::service& srv);
    ~AdminService();
    
private:
    virtual void index() override;
};

#endif
