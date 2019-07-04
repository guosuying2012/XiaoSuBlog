#include "AdminService.h"
#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>
#include <StringTemplate/StringTemplate.hpp>

using namespace StringTemplate;

AdminService::AdminService(cppcms::service& srv)
    :BaseService(srv)
{
    dispatcher().map("GET", "", &AdminService::index, this);
    mapper().root("/xiaosu");
}

AdminService::~AdminService()
{

}

void AdminService::index()
{
    Template tpl("./admin/index.html");
    tpl.set("title", "XiaoSu");
    tpl.render(response(200, "text/html").out(), true);
}

