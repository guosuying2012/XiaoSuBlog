#ifndef APPLICATION_SERVICE_H
#define APPLICATION_SERVICE_H

#include "BaseService.h"

class ApplicationService : public BaseService
{
public:
    ApplicationService(cppcms::service& srv);
    ~ApplicationService();
    void index();
    
};

#endif  //APPLICATION_SERVICE_H