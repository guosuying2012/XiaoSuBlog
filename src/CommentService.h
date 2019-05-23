#ifndef COMMENT_SERVICE_H
#define COMMENT_SERVICE_H

#include "BaseService.h"

class CommentService : public BaseService
{
public:
    CommentService(cppcms::service& srv);
    ~CommentService();
    
private:
    virtual void index() override;
    void commentByArticleId(int nArticleId);
};

#endif