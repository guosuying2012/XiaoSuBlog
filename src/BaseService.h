#ifndef BASE_SERVICE_H
#define BASE_SERVICE_H 

#include <cppcms/application.h>
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
	virtual cppdb::session& database();
	virtual void clear();
protected:
	void init();
	void buildDbConnString();
private:
	std::shared_ptr<cppdb::session> m_pDBInstence;
	std::string m_strConnStr;
};

#endif	//BASESERVICE.H 
