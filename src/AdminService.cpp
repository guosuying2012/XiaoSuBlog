#include "AdminService.h"

#include "support.h"
#include "DatabaseUtils.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>  
#include <cppcms/url_mapper.h>

#include <vector>

AdminService::AdminService(cppcms::service& srv)
    :BaseService(srv)
{
    m_nIndex = -1;
    dispatcher().map("GET", "", &AdminService::index, this);
    dispatcher().map("GET", "/publish", &AdminService::publish, this);
    dispatcher().map("GET", "/article", &AdminService::article, this);
    dispatcher().map("GET", "/users", &AdminService::users, this);
    dispatcher().map("GET", "/system", &AdminService::system, this);
    mapper().root("/xiaosu");
}

AdminService::~AdminService()
{

}

void AdminService::index()
{
    Template tpl("./admin/index.html");
    tpl.set("function", "控制台");
    m_nIndex = 0;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::publish()
{
    Template tpl("./admin/publish.html");
    sorts vecRes;

    vecRes.clear();
    tpl.set("function", "发表文章");
    m_nIndex = 1;
    renderMenu(tpl);

    DatabaseUtils::queryAllSorts(database(), false, vecRes);
    auto sorts_block = tpl.block("option").repeat(vecRes.size());
    for (int i = 0; i < vecRes.size(); ++i)
    {
        sorts_block.set("value", vecRes.at(i).strName);
        sorts_block.set("value_id", vecRes.at(i).nId);
        sorts_block = sorts_block.next();
    }

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::article()
{
    Template tpl("./admin/article.html");
    tpl.set("function", "文章管理");
    m_nIndex = 2;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::users()
{
    Template tpl("./admin/users.html");
    tpl.set("function", "用户管理");
    m_nIndex = 3;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::system()
{
    Template tpl("./admin/system.html");
    tpl.set("function", "系统设置");
    m_nIndex = 4;
    renderMenu(tpl);

    tpl.render(response(200, "text/html").out(), true);
}

void AdminService::renderMenu(Template& tpl)
{
    std::vector<std::string> vecMenuTitles{ "控制台", "发表文章", "文章管理", "用户管理", "系统设置" };
    std::vector<std::string> vecMenuLinks{ "/xiaosu/admin", "/xiaosu/admin/publish", "/xiaosu/admin/article", "/xiaosu/admin/users", "/xiaosu/admin/system" };
    std::vector<std::string> vecMenuIcons{ "home", "pen-tool", "inbox", "users", "settings" };

    if (vecMenuTitles.size() != vecMenuLinks.size())
    {
        return;
    }

    tpl.set("title", "XiaoSu");
    
    auto group = tpl.block("menu_group").repeat(vecMenuTitles.size());
    for (int i = 0; i < vecMenuTitles.size(); ++i)
    {
        group.set("menu_text", vecMenuTitles.at(i));
        group.set("menu_link", vecMenuLinks.at(i));
        group.set("menu_icon", vecMenuIcons.at(i));

        if (i == m_nIndex)
        {
            group.set("menu_active", "is-active");
        }

        group = group.next();
    }
}

