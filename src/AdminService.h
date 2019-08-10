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

    void signin();
    void signout();

    void admin_publish();
    void admin_articles();
    void admin_users();
    void admin_system();

    void article_edit(std::string strId);
    void article_delete(std::string strId);
    void article_verify(std::string strId);

    void user_edit(int nId);
    void user_disable(int nId);

    void message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText);

    void postUser();
    void postArticle();
    void uploadImages();

private:
    
};

#endif
