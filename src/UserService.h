#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "BaseService.h"

class UserService : public BaseService
{
public:
    UserService(cppcms::service& srv);
    ~UserService();
    
private:
    virtual void index() override;
    void userById(int nId);
};

#endif  //USER_SERVICE_H