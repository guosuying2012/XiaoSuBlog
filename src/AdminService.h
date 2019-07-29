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

    void admin_publish();
    void admin_articles();
    void admin_users();
    void admin_system();

    void edit(int nId);
    void message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText);

    void postArticle();
    void uploadImages();

private:
    std::string m_strPostToken;
};

#endif
