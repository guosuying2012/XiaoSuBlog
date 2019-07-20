#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include "BaseService.h"
#include <StringTemplate/StringTemplate.hpp>
using namespace StringTemplate;

class AdminService : public BaseService
{
public:
    AdminService(cppcms::service& srv);
    ~AdminService();
    
private:
    virtual void index() override;

    void publish();
    void article();
    void users();
    void system();

    void edit();
    void message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText);

    void postArticle();
    void uploadImages();

    void renderMenu(Template& tpl);

private:
    int m_nIndex;
    std::string m_strPostToken;
};

#endif
