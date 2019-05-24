#include "UserService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

UserService::UserService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &UserService::index, this);
    dispatcher().map("GET", "/getUserById/(\\d+)", &UserService::userById, this, 1);
    mapper().root("/xiaosu");
}

UserService::~UserService()
{
}

void UserService::index()
{
    response().out() << "UserService";
}

void UserService::userById(int nId)
{
    user record;
    record.clear();

    if (nId <= 0)
    {
        json()["data"] = "null";
        json()["error"] = "未找到相关用户!";
        response().out() << json();
        return;
    }
    
    try
    {
        DatabaseUtils::queryUserById(database(), nId, record);
    }
    catch (cppdb::cppdb_error const& e)
    {
        json()["data"] = "null";
        json()["error"] = e.what();
        response(500).out() << json();
        return;
    }

    json()["data"] = record;
    json()["error"] = "null";
    response().out() << json();
}
