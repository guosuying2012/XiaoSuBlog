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

    void article_edit(int nId);
    void article_delete(int nId);
    void article_verify(int nId);

    void user_edit(int nId);
    void user_disable(int nId);

    void message(std::string strMsgType, std::string strMsgTitle, std::string strMsgText);

    void postUser();
    void postArticle();
    void uploadImages();

private:
    
};

#endif
