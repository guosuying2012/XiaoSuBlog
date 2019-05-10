#include "BaseService.h"
#include <cppcms/service.h>
#include <cppdb/frontend.h>
#include <sstream>

BaseService::BaseService(cppcms::service &s)
    :application(s)
{
    m_pDBPool = nullptr;
    m_pDBInstence = nullptr;

    init();
}

BaseService::~BaseService()
{
    close();
}

cppdb::session& BaseService::database()
{
    if(!m_pDBInstence->is_open())
    {
        m_pDBInstence = std::make_shared<cppdb::session>(m_pDBPool->open());
    }
    return *m_pDBInstence;
}

void BaseService::close()
{
    m_pDBInstence->close();
}

void BaseService::init()
{
    std::string strConnectString;
    strConnectString.clear();

    buildDbConnString(strConnectString);
    m_pDBPool = cppdb::pool::create(strConnectString);
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
    oss << driver << ":host=" << host <<";database=" << db << ";user=" << user << ";password=" << password << ";";

    strResConn = oss.str();
}