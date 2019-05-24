#include "BaseService.h"
#include <cppcms/service.h>
#include <cppcms/http_context.h>
#include <cppdb/frontend.h>
#include <sstream>

BaseService::BaseService(cppcms::service &s)
    :application(s)
{
    m_pDBPool = nullptr;
    m_pDBInstence = nullptr;
    m_strConnectString.clear();

    init();
}

BaseService::~BaseService()
{
    close();
}

cppcms::http::response& BaseService::response(int nCode, std::string const& strReponseType)
{
    context().response().status(nCode);
    context().response().content_type(strReponseType);
    context().response().add_header("Accept", strReponseType);
    return context().response();
}

cppdb::session& BaseService::database()
{
    if (!m_pDBInstence->is_open())
    {
        m_pDBInstence->open(m_strConnectString);
    }
    return *m_pDBInstence;
}

cppcms::json::value& BaseService::json()
{
    return m_JsonValue;
}

void BaseService::close()
{
    if (m_pDBInstence->is_open())
    {
        m_pDBInstence->close();
    }
}

void BaseService::init()
{
    buildDbConnString(m_strConnectString);
    m_pDBPool = cppdb::pool::create(m_strConnectString);
    m_pDBInstence = std::make_shared<cppdb::session>(m_pDBPool->open());
}

void BaseService::buildDbConnString(std::string& strResConn)
{
    std::ostringstream oss;
    strResConn.clear();
    oss.clear();

    std::string driver = settings().get<std::string>("database.driver");
    std::string host = settings().get<std::string>("database.host");
    std::string db = settings().get<std::string>("database.db");
    std::string user = settings().get<std::string>("database.user");
    std::string password= settings().get<std::string>("database.password");
    oss << driver << ":host=" << host <<";database=" << db << ";user=" << user << ";password=" << password << ";set_charset_name=utf8;";

    strResConn = oss.str();
}