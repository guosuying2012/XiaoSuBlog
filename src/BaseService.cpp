#include "BaseService.h"
#include <cppcms/service.h>
#include <cppdb/frontend.h>
#include <sstream>

BaseService::BaseService(cppcms::service &s)
    :application(s)
{
    m_strConnStr.clear();
    m_pDBInstence = nullptr;

    init();
}

BaseService::~BaseService()
{
}

cppdb::session& BaseService::database()
{
    if(!m_pDBInstence->is_open())
	{
		m_pDBInstence->open(m_strConnStr);
	}
	return *m_pDBInstence;
}

void BaseService::clear()
{
    m_pDBInstence->close();
}

void BaseService::init()
{
	m_pDBInstence = std::make_shared<cppdb::session>();
	buildDbConnString();
}

void BaseService::buildDbConnString()
{
	std::ostringstream oss;

	if (!m_strConnStr.empty())
	{
		return;
	}
	oss.clear();

	std::string driver = settings().get<std::string>("database.driver");
	std::string host = settings().get<std::string>("database.host");
	std::string db = settings().get<std::string>("database.db");
	std::string user = settings().get<std::string>("database.user");
	std::string password= settings().get<std::string>("database.password");
	oss << driver << ":database=" << db << ";user=" << user << ";password=" << password << ";@pool_size=10";

	m_strConnStr = oss.str();
}