#include "ApplicationService.h"
#include "IndexService.h"
#include "ArticleService.h"
#include "CategoryService.h"
#include "SettingsService.h"
#include "UserService.h"

#include <cppcms/url_mapper.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>

ApplicationService::ApplicationService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &ApplicationService::index, this);

    attach(new IndexService(srv), "index", "/index{1}", "/index((/?.*))", 1);
    attach(new ArticleService(srv), "article", "/article{1}", "/article((/?.*))", 1);
    attach(new CategoryService(srv), "category", "/category{1}", "/category((/?.*))", 1);
    attach(new SettingsService(srv), "settings", "/settings{1}", "/settings((/?.*))", 1);
    attach(new UserService(srv), "user", "/user{1}", "/user((/?.*))", 1);

    mapper().root("/xiaosu");
}

ApplicationService::~ApplicationService()
{
}

void ApplicationService::index()
{
    //API LIST
    response().out() << "ApplicationService";
}
