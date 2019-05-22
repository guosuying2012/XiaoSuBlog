#ifndef ARTICLE_SERVICE_H
#define ARTICLE_SERVICE_H

#include "BaseService.h"

class ArticleService : public BaseService
{
public:
    ArticleService(cppcms::service& srv);
    ~ArticleService();
    
private:
    virtual void index() override;
    void articleById(int nArticleId);
};

#endif