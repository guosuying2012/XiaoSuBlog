#ifndef BASE_SERVICE_H
#define BASE_SERVICE_H 

#include <cppcms/application.h>
#include <cppcms/http_response.h>
#include <cppcms/json.h>
#include <cppdb/backend.h>
#include <cppdb/pool.h>
#include <string>

namespace cppdb
{
    class session;
}

class BaseService : public cppcms::application
{
public:
    BaseService(cppcms::service& s);
    virtual ~BaseService();
    virtual cppcms::http::response& response(int nCode = 200, std::string const& strReponseType = "application/json");
    virtual cppdb::session& database();
    virtual cppcms::json::value& json();
    virtual void close();
    virtual void index() = 0;

protected:
    void init();
    void buildDbConnString(std::string& strResConn);
    
private:
    std::string m_strConnectString;
    cppdb::pool::pointer m_pDBPool;
    cppcms::json::value m_JsonValue;
    std::shared_ptr<cppdb::session> m_pDBInstence;
};

#endif  //BASESERVICE.H 
